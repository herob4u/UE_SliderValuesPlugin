// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliderLinkBase.h"
#include "SliderLinkComposite.generated.h"

/**
 * 
 */
USTRUCT()
struct SLIDERVALUES_API FSliderLinkComposite : public FSliderLinkBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UCurveTable* LinkedSlidersCurves;
};

template<>
struct FSliderEvaluator<FSliderLinkComposite>
{
	static decltype(auto) Evaluate(float sliderValue, const FSliderLinkComposite& sliderLink)
	{
		TArray<float> output;
		
	}
};
