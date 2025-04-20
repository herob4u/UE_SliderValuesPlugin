// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliderLinkBase.h"
#include "SliderLinkMaterial.generated.h"

USTRUCT()
struct SLIDERVALUES_API FSliderLinkMaterialScalar : public FSliderLinkBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> LinkedParams;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* StateValues;
};

USTRUCT()
struct SLIDERVALUES_API FSliderLinkMaterialColor : public FSliderLinkBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> LinkedParams;

	UPROPERTY(EditDefaultsOnly)
	UCurveLinearColor* StateValues;
};