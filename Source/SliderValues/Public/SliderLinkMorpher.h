// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliderLinkBase.h"
#include "SliderLinkMorpher.generated.h"

USTRUCT()
struct SLIDERVALUES_API FMorpherState : public FSliderStateBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UCurveFloat* StateValues;
};
/**
 * 
 */
USTRUCT()
struct SLIDERVALUES_API FSliderLinkMorpher : public FSliderLinkBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="SliderValues.SliderValuesConfig.GetAllMorphTargets"))
	TArray<FName> LinkedMorphTargets;

	UPROPERTY(EditAnywhere)
	UCurveFloat* StateValues;
	//TArray<FMorpherState> States;
};
