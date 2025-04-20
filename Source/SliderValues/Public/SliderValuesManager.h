// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SliderValuesComponent.h"
#include "SliderValuesManager.generated.h"

class USliderValuesComponent;

/**
 * 
 */
UCLASS()
class SLIDERVALUES_API USliderValuesManager final : public UObject
{
	GENERATED_BODY()

public:
	template<typename T>
	using Array = TArray<T, TInlineAllocator<64>>;

	USliderValuesManager();
	void PostInitProperties() override;

	void Update();
	void DrawImGui();
private:
	void DrawSliderComponent(USliderValuesComponent* sliderValuesComp);
	void DrawComponentData(const USliderValuesComponent::FData& data);

	void UpdateComponents(const Array<int32>& indices);
	void UpdateClearDirty(const Array<int32>& indices);

	void OnComponentCreated(USliderValuesComponent* sliderValuesComp);
	void OnComponentDestroyed(USliderValuesComponent* sliderValuesComp);
	void OnComponentUpdated(USliderValuesComponent* sliderValuesComp);

	void AllocateData(USliderValuesComponent* sliderValuesComp);
	void DeallocateData(USliderValuesComponent* sliderValuesComp);

	int32 FindData(int32 id) const;
	void GetDirtyIndices(Array<int32>& outIndices);

	Array<USliderValuesComponent::FData> m_ComponentsData;
	TSet<int32> m_DestroyedEvents;
};
