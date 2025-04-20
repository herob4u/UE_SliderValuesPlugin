// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SliderLinkBase.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "StructUtils/InstancedStruct.h"
#include "SliderDefinitions.generated.h"

USTRUCT()
struct FSliderDefinition2
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SliderId;

	UPROPERTY(EditDefaultsOnly, meta = (ShowOnlyInnerProperties, FullyExpand = true, ExcludeBaseStruct), NoClear)
	TArray<TInstancedStruct<FSliderLinkBase>> Links;
};

USTRUCT(BlueprintType)
struct FSliderDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly)
	float DefaultValue;

	UPROPERTY(EditDefaultsOnly, meta = (ShowOnlyInnerProperties, FullyExpand = true, ExcludeBaseStruct), NoClear)
	TArray<TInstancedStruct<FSliderLinkBase>> Links;
};
using SliderLinks = TArray<TInstancedStruct<FSliderLinkBase>>;

/**
 * 
 */
UCLASS()
class SLIDERVALUES_API USliderDefinitions : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FSliderDefinition> Sliders;

	UPROPERTY(EditDefaultsOnly)
	TArray<FCurveTableRowHandle> CompositeSliders;
protected:

	UFUNCTION()
	TArray<FName> GetSliderIds() const
	{
		TArray<FName> result;
		result.Reserve(Sliders.Num());
		for (const FSliderDefinition& definition : Sliders)
		{
			//result.Add(definition.SliderId);
		}

		return result;
	}
};
