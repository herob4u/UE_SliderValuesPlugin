// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SliderDefinitions.h"
#include "Components/ActorComponent.h"
#include "SliderValuesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class SLIDERVALUES_API USliderValuesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USliderValuesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta = (RequiredAssetDataTags = "RowStructure=/Script/SliderValues.SliderDefinition"))
	void RegisterSliderDefinitions(UDataTable* sliderDefinitionsDt);

	int32 GetSliderIdx(const FName& sliderId) const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void BuildSliderValues(const TArray<FName>& sliderIds, const UDataTable& sliderDefinitionsDt);
	void BuildSliderIndices(const TArray<FName>& sliderIds);

	UPROPERTY(EditDefaultsOnly)
	UDataTable* SliderDefinitions;

	TArray<float> SliderValues;

	TMap<FName, int32> SliderIndexMap;

	UCurveTable* CompositeSliders;
};
