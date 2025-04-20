// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
private:
	void DrawImGui();
	void DrawSliderComponent(USliderValuesComponent* sliderValuesComp);

	void UpdateComponents(const Array<int32>& indices);
	void UpdateClearDirty(const Array<int32>& indices);

	void OnComponentCreated(USliderValuesComponent* sliderValuesComp);
	void OnComponentDestroyed(USliderValuesComponent* sliderValuesComp);
	void OnComponentChanged(USliderValuesComponent* sliderValuesComp);

	int32 FindUpdateInfo(int32 id) const;
	void GetDirtyIndices(Array<int32>& outIndices);

	struct ComponentUpdateInfo
	{
		static constexpr int32 INVALID_ID = 0xffffffff;

		int32 m_Id = INVALID_ID;
		bool m_IsDirty : 1 = false;
		// Other meta data
	};

	UPROPERTY(transient)
	TArray<USliderValuesComponent*> m_Components;

	Array<ComponentUpdateInfo> m_UpdateInfo;
	TSet<int32> m_DestroyedEvents;
};
