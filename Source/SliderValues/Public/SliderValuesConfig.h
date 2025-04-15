// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SliderValuesConfig.generated.h"

/**
 * 
 */
UCLASS()
class SLIDERVALUES_API USliderValuesConfig : public UObject
{
	GENERATED_BODY()
public:

	static const USliderValuesConfig& GetCurrentConfig();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USkeletalMesh>> MorphReferenceMeshes;

	UFUNCTION()
	static TArray<FName> GetAllMorphTargets();
#endif
};
