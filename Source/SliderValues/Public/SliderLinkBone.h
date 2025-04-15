// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliderLinkBase.h"
#include "Curves/CurveVector.h"
#include "SliderLinkBone.generated.h"

USTRUCT()
struct SLIDERVALUES_API FBoneState : public FSliderStateBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UCurveVector* PositionValues;

	UPROPERTY(EditAnywhere)
	UCurveVector* EulerValues;

	UPROPERTY(EditAnywhere)
	UCurveVector* ScaleValues;
};

/**
 * 
 */
USTRUCT()
struct SLIDERVALUES_API FSliderLinkBone : public FSliderLinkBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FName> LinkedBones;

	UPROPERTY(EditAnywhere)
	UCurveVector* PositionStateValues;

	UPROPERTY(EditAnywhere)
	UCurveVector* EulerStateValues;

	UPROPERTY(EditAnywhere)
	UCurveVector* ScaleStateValues;

	//UPROPERTY(EditAnywhere)
	//TArray<FBoneState> States;
};

template<>
struct FSliderEvaluator<FSliderLinkBone>
{
	static decltype(auto) Evaluate(float sliderValue, const FSliderLinkBone& sliderLink)
	{
		return FTransform::Identity;
	}
};