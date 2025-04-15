// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliderLinkBase.h"
#include "SliderLinkMaterial.generated.h"

struct SLIDERVALUES_API FMaterialScalarState
{
};

struct SLIDERVALUES_API FMaterialVectorState
{

};

/**
 * 
 */
USTRUCT()
struct SLIDERVALUES_API FSliderLinkMaterial : public FSliderLinkBase
{
	GENERATED_BODY()
	
};

template<>
struct FSliderEvaluator<FSliderLinkMaterial>
{
	static decltype(auto) Evaluate(float state, const FSliderLinkMaterial& link)
	{
		return 1.2f;
	}
};