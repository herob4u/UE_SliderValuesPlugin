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
			for (const auto& data : m_ComponentsData)
			{
				if (USliderValuesComponent* sliderValuesComp = data.m_Component)
				{
					if (ImGui::CollapsingHeader(TCHAR_TO_UTF8(*sliderValuesComp->GetOwner()->GetName()), ImGuiTreeNodeFlags_Framed))
					{
						DrawSliderComponent(sliderValuesComp);
						ImGui::Separator();
						DrawComponentData(data);
					}
				}
			}

			ImGui::SeparatorText("Misc.");
			ImGui::Text("Num Component Data: %i", m_ComponentsData.Num());
			ImGui::Text("Bytes Component Data: %llu", m_ComponentsData.NumBytes());

			ImGui::End();
		}
	}
}

void USliderValuesManager::DrawSliderComponent(USliderValuesComponent* sliderValuesComp)
{
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

			float rangeMin = sliderDefinitions[sliderIdx].SliderMin;
			float rangeMax = sliderDefinitions[sliderIdx].SliderMax;

			if (ImGui::SliderFloat(nameCStr, &sliderInstances[sliderIdx].Value, rangeMin, rangeMax))
			{
				OnComponentUpdated(sliderValuesComp);
			}
		}
	}
}

void USliderValuesManager::DrawComponentData(const USliderValuesComponent::FData& data)
{
	ImGui::SeparatorText("Data");
	ImGui::Text("ID: %i", data.m_Id);
	ImGui::Text("IsDirty: %s", data.m_IsDirty ? "true" : "false");
}

void USliderValuesManager::UpdateComponents(const Array<int32>& indices)
{
	for (int32 idx : indices)
	{
		USliderValuesComponent* sliderValuesComp = m_ComponentsData[idx].m_Component;
		check(sliderValuesComp);

		sliderValuesComp->UpdateSliders();
	}
}

void USliderValuesManager::UpdateClearDirty(const Array<int32>& indices)
{
	for (int32 idx : indices)
	{
		m_ComponentsData[idx].m_IsDirty = false;
	}
}

void USliderValuesManager::OnComponentCreated(USliderValuesComponent* sliderValuesComp)
{
	check(sliderValuesComp);

	int32 existingIdx = FindData(sliderValuesComp->GetUniqueID());
	if (existingIdx != INDEX_NONE)
	{
		// Already exists?!
		check(false);
		return;
	}

	AllocateData(sliderValuesComp);
}

void USliderValuesManager::OnComponentDestroyed(USliderValuesComponent* sliderValuesComp)
{
	check(sliderValuesComp);
	// Expected to happen rarely, so let's just remove from array directly
	int32 id = sliderValuesComp->GetUniqueID();
	int32 idx = FindData(id);
	check(idx != INDEX_NONE); // Was never rregistered?!

	if (idx != INDEX_NONE)
	{
		DeallocateData(sliderValuesComp);
	}
}

void USliderValuesManager::OnComponentUpdated(USliderValuesComponent* sliderValuesComp)
{
	int32 idx = FindData(sliderValuesComp->GetUniqueID());
	if (idx != INDEX_NONE)
	{
		m_ComponentsData[idx].m_IsDirty = true;
	}
}

void USliderValuesManager::AllocateData(USliderValuesComponent* sliderValuesComp)
{
	int32 idx = FindData(USliderValuesComponent::FData::INVALID_ID);
	if (idx == INDEX_NONE)
	{
		idx = m_ComponentsData.AddUninitialized();
	}

	m_ComponentsData[idx].m_Component = sliderValuesComp;
	m_ComponentsData[idx].m_Id = sliderValuesComp->GetUniqueID();
	m_ComponentsData[idx].m_IsDirty = false;
}

void USliderValuesManager::DeallocateData(USliderValuesComponent* sliderValuesComp)
{
	verify(sliderValuesComp);

	if (sliderValuesComp)
	{
		int32 idx = FindData(sliderValuesComp->GetUniqueID());
		checkf(idx != INDEX_NONE, TEXT("Mistracked allocation! Trying to deallocate component data that doesn't exist!"));

		m_ComponentsData[idx].m_Component = nullptr;
		m_ComponentsData[idx].m_Id = USliderValuesComponent::FData::INVALID_ID;
	}
}

int32 USliderValuesManager::FindData(int32 id) const
{
	for (int32 i = 0; i < m_ComponentsData.Num(); ++i)
	{
		if (m_ComponentsData[i].m_Id == id)
			return i;
	}

	return INDEX_NONE;
}

void USliderValuesManager::GetDirtyIndices(Array<int32>& outIndices)
{
	for (int32 i = 0; i < m_ComponentsData.Num(); ++i)
	{
		if (m_ComponentsData[i].m_IsDirty == true)
		{
			outIndices.Add(i);
		}
	}
}
