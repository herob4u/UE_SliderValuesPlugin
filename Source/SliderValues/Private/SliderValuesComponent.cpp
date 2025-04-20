// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderValuesComponent.h"
#include "Engine/AssetManager.h"

TDelegate<void(USliderValuesComponent*)> USliderValuesComponent::OnCreated;
TDelegate<void(USliderValuesComponent*)> USliderValuesComponent::OnDestroyed;
TDelegate<void(USliderValuesComponent*)> USliderValuesComponent::OnUpdated;

// Sets default values for this component's properties
USliderValuesComponent::USliderValuesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called every frame
void USliderValuesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If tick enabled, always update. Otherwise let Manager update.
	UpdateSliders();
}

void USliderValuesComponent::UpdateSliders()
{
}

// Called when the game starts
void USliderValuesComponent::BeginPlay()
{
	Super::BeginPlay();

	OnCreated.ExecuteIfBound(this);
	
	if (SliderDefinitionsDt.Get())
	{
		RegisterSliderDefinitions(SliderDefinitionsDt.Get());
	}
	else if (!SliderDefinitionsDt.IsNull())
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad({ SliderDefinitionsDt.ToSoftObjectPath() }, [this] {
			RegisterSliderDefinitions(SliderDefinitionsDt.Get());
			});
	}
}

void USliderValuesComponent::EndPlay(EEndPlayReason::Type reason)
{
	OnDestroyed.ExecuteIfBound(this);

	Super::EndPlay(reason);
}


void USliderValuesComponent::BuildSliderInstances(const TArray<FName>& sliderIds, const UDataTable& sliderDefinitionsDt)
{
	SliderInstances.Reserve(sliderIds.Num());
	SliderDefinitions.Reserve(sliderIds.Num());

	for (int sliderIdx = 0; sliderIdx < sliderIds.Num(); ++sliderIdx)
	{
		FSliderDefinition* definition = (FSliderDefinition*)sliderDefinitionsDt.FindRowUnchecked(sliderIds[sliderIdx]);
		SliderInstances.Emplace(definition->Links, GetDefaultValue(*definition));
		SliderDefinitions.Emplace(*definition);
	}
}

void USliderValuesComponent::BuildSliderIndices(const TArray<FName>& sliderIds)
{
	SliderIndexMap.Reserve(sliderIds.Num());
	for (int sliderIdx = 0; sliderIdx < sliderIds.Num(); ++sliderIdx)
	{
		SliderIndexMap.Add(sliderIds[sliderIdx], sliderIdx);
	}
}


void USliderValuesComponent::RegisterSliderDefinitions(UDataTable* sliderDefinitionsDt)
{
	if (sliderDefinitionsDt)
	{
		check(sliderDefinitionsDt->GetRowStruct() == FSliderDefinition::StaticStruct());

		TArray<FName> rows = sliderDefinitionsDt->GetRowNames();
		BuildSliderInstances(rows, *sliderDefinitionsDt);
		BuildSliderIndices(rows);
		SliderIds = MoveTemp(rows);

		OnUpdated.ExecuteIfBound(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SliderValuesComponent has no slider definitions! Component has no effect"));
	}
}

float USliderValuesComponent::GetDefaultValue(const FSliderDefinition& sliderDefinition)
{
	return FMath::Clamp(sliderDefinition.DefaultValue, sliderDefinition.SliderMin, sliderDefinition.SliderMax);
}

int32 USliderValuesComponent::GetSliderIdx(const FName& sliderId) const
{
	return SliderIndexMap.FindRef(sliderId, INDEX_NONE);
}

float USliderValuesComponent::GetSliderValue(const FName& sliderId) const
{
	const int32 sliderIdx = GetSliderIdx(sliderId);
	if (sliderIdx != INDEX_NONE)
	{
		return GetSliderValue(sliderIdx);
	}

	return 0.0f;
}

float USliderValuesComponent::GetSliderValue(int32 sliderIdx) const
{
	if (SliderInstances.IsValidIndex(sliderIdx))
	{
		return SliderInstances[sliderIdx].Value;
	}

	return 0.0f;
}

TArray<float> USliderValuesComponent::GetSliderValues() const
{
	TArray<float> values;
	values.Reserve(SliderInstances.Num());

	for (const FSliderInstance& sliderInstance : SliderInstances)
	{
		values.Add(sliderInstance.Value);
	}

	return values;
}

void USliderValuesComponent::ResetDefaultValues()
{
	for (int32 sliderIdx = 0; sliderIdx < SliderDefinitions.Num(); ++sliderIdx)
	{
		ResetDefaultValue(sliderIdx, false);
	}

	OnValuesChanged();
}

void USliderValuesComponent::ResetDefaultValue(const FName& sliderId, bool sendChangedEvent)
{
	int32 sliderIdx = SliderIndexMap.FindRef(sliderId, INDEX_NONE);
	if (sliderIdx != INDEX_NONE)
	{
		ResetDefaultValue(sliderIdx, sendChangedEvent);
	}
}

void USliderValuesComponent::ResetDefaultValue(int32 sliderIdx, bool sendChangedEvent)
{
	if (SliderDefinitions.IsValidIndex(sliderIdx))
	{
		if (!SliderInstances.IsValidIndex(sliderIdx))
		{
			check(false);
			return;
		}

		SliderInstances[sliderIdx].Value = GetDefaultValue(SliderDefinitions[sliderIdx]);

		if(sendChangedEvent)
			OnValuesChanged();
	}
}

void USliderValuesComponent::SetSliderValue(const FName sliderId, float value, bool sendChangedEvent)
{
	int32 sliderIdx = SliderIndexMap.FindRef(sliderId, INDEX_NONE);
	if (sliderIdx != INDEX_NONE)
	{
		SetSliderValue(sliderIdx, value, sendChangedEvent);
	}
}

void USliderValuesComponent::SetSliderValue(int32 sliderIdx, float value, bool sendChangedEvent)
{
	if (!SliderInstances.IsValidIndex(sliderIdx))
	{
		check(false);
		return;
	}

	SliderInstances[sliderIdx].Value = value;

	if(sendChangedEvent)
		OnValuesChanged();
}

void USliderValuesComponent::SetSliderValues(const TMap<FName, float>& sliderIdToValues)
{
	for (auto pair : sliderIdToValues)
	{
		SetSliderValue(pair.Key, pair.Value, false);
	}

	OnValuesChanged();
}

void USliderValuesComponent::SetSliderValues(const TArray<float>& sliderValues)
{
	check(sliderValues.Num() == SliderInstances.Num());
	for (int32 sliderIdx = 0; sliderIdx < sliderValues.Num(); ++sliderIdx)
	{
		SliderInstances[sliderIdx].Value = sliderValues[sliderIdx];
	}

	OnValuesChanged();
}

void USliderValuesComponent::OnValuesChanged()
{
	OnUpdated.ExecuteIfBound(this);
}

