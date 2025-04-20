// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderValuesManager.h"
#include "SliderValuesComponent.h"

#include "imgui.h"

USliderValuesManager::USliderValuesManager()
{
}

void USliderValuesManager::PostInitProperties()
{
	Super::PostInitProperties();

	// Setup callbacks
	USliderValuesComponent::OnCreated.BindUObject(this, &USliderValuesManager::OnComponentCreated);
	USliderValuesComponent::OnDestroyed.BindUObject(this, &USliderValuesManager::OnComponentDestroyed);
}

void USliderValuesManager::Update()
{
	Array<int32> indicesToUpdate;
	GetDirtyIndices(indicesToUpdate);

	UpdateClearDirty(indicesToUpdate); // Already access m_UpdateInfo above, keep accessing it.
	UpdateComponents(indicesToUpdate);

	//Update_SliderValues();
}

void USliderValuesManager::DrawImGui()
{
	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		if (ImGui::Begin("SliderValuesManager"))
		{
			for (USliderValuesComponent* sliderComponent : m_Components)
			{
				DrawSliderComponent(sliderComponent);
			}
		}
	}
}

void USliderValuesManager::DrawSliderComponent(USliderValuesComponent* sliderValuesComp)
{
	if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*sliderValuesComp->GetOwner()->GetName()), ImGuiTreeNodeFlags_Framed))
	{
		const auto& sliderDefinitions = sliderValuesComp->GetSliderDefinitions();
		auto sliderInstances = sliderValuesComp->GetSliderInstances();

		if (sliderDefinitions.Num() != sliderInstances.Num())
		{
			ImGui::Text("Mismatching slider definitions [%i] versus instance [%i] count!", sliderDefinitions.Num(), sliderInstances.Num());
			return;
		}

		for (int32 sliderIdx = 0; sliderIdx < sliderDefinitions.Num(); ++sliderIdx)
		{
			FString nameStr = sliderDefinitions[sliderIdx].DisplayName.ToString();
			const char* nameCStr = TCHAR_TO_UTF8(*nameStr);

			static float rangeMin = 0.0f;
			static float rangeMax = 1.0f;

			ImGui::SliderFloat(nameCStr, &sliderInstances[sliderIdx].Value, rangeMin, rangeMax);
		}
	}
}

void USliderValuesManager::UpdateComponents(const Array<int32>& indices)
{
	for (int32 idx : indices)
	{
		USliderValuesComponent* sliderValuesComp = m_Components[idx];
		check(sliderValuesComp);

		sliderValuesComp->UpdateSliders();
	}
}

void USliderValuesManager::UpdateClearDirty(const Array<int32>& indices)
{
	for (int32 idx : indices)
	{
		m_UpdateInfo[idx].m_IsDirty = false;
	}
}

void USliderValuesManager::OnComponentCreated(USliderValuesComponent* sliderValuesComp)
{
	check(sliderValuesComp);

	m_Components.AddUnique(sliderValuesComp);

	ComponentUpdateInfo updateInfo;
	updateInfo.m_Id = sliderValuesComp->GetUniqueID();

	m_UpdateInfo.Push(updateInfo);

	check(m_Components.Num() == m_UpdateInfo.Num());
}

void USliderValuesManager::OnComponentDestroyed(USliderValuesComponent* sliderValuesComp)
{
	check(sliderValuesComp);
	// Expected to happen rarely, so let's just remove from array directly
	int32 id = sliderValuesComp->GetUniqueID();
	int32 idx = FindUpdateInfo(id);
	if (m_UpdateInfo.IsValidIndex(idx))
	{
		m_UpdateInfo.RemoveAtSwap(idx);
	}

	if (m_Components.IsValidIndex(idx))
	{
		check(m_Components[idx]->GetUniqueID() == id);
		m_Components.RemoveAtSwap(idx);
	}
	else
	{
		check(false);
	}

	check(m_Components.Num() == m_UpdateInfo.Num());
}

int32 USliderValuesManager::FindUpdateInfo(int32 id) const
{
	for (int32 i = 0; i < m_UpdateInfo.Num(); ++i)
	{
		if (m_UpdateInfo[i].m_Id == id)
			return i;
	}

	return INDEX_NONE;
}

void USliderValuesManager::GetDirtyIndices(Array<int32>& outIndices)
{
	for (int32 i = 0; i < m_UpdateInfo.Num(); ++i)
	{
		if (m_UpdateInfo[i].m_IsDirty == true)
		{
			outIndices.Add(i);
		}
	}
}
