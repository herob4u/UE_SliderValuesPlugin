// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SliderLinkBase.generated.h"

template<typename TSliderLink>
struct FSliderLinkTrait
{
	using ValueType = float;
};

template<typename TSliderLink>
struct FSliderEvaluator
{
	using ValueType = FSliderLinkTrait<TSliderLink>::ValueType;
	static decltype(auto) Evaluate(float sliderValue, const TSliderLink& sliderLink)
	{
		static_assert(TIsSame<ValueType, void>::Value == false, "SliderLinkTrait not specialized!");
		checkf(false, TEXT("SliderEvaluator must be specialized for your respective type!"));
	}
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct SLIDERVALUES_API FSliderLinkBase
{
	GENERATED_BODY()
};