// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderValuesComponent.h"

// Sets default values for this component's properties
USliderValuesComponent::USliderValuesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void USliderValuesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Called when the game starts
void USliderValuesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USliderValuesComponent::BuildSliderValues(const TArray<FName>& sliderIds, const UDataTable& sliderDefinitionsDt)
{
	SliderValues.SetNumUninitialized(sliderIds.Num());

	for (int sliderIdx = 0; sliderIdx < sliderIds.Num(); ++sliderIdx)
	{
		FSliderDefinition* definition = (FSliderDefinition*)sliderDefinitionsDt.FindRowUnchecked(sliderIds[sliderIdx]);
		SliderValues[sliderIdx] = definition->DefaultValue;
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
		BuildSliderValues(rows, *sliderDefinitionsDt);
		BuildSliderIndices(rows);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SliderValuesComponent has no slider definitions! Component has no effect"));
	}
}

int32 USliderValuesComponent::GetSliderIdx(const FName& sliderId) const
{
	const int32* idx = SliderIndexMap.Find(sliderId);
	return idx ? *idx : INDEX_NONE;
}

