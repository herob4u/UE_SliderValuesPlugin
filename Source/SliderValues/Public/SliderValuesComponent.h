// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SliderDefinitions.h"
#include "Components/ActorComponent.h"
#include "SliderValuesComponent.generated.h"

struct FSliderInstance
{
	FSliderInstance(const SliderLinks& links, float defaultValue = 0.f)
		: Value(defaultValue)
		, Links(links)
	{
	}

	float Value;
	const SliderLinks Links;
};

// Wrapper to provide mutable access to array elements, but not resizable
class FConstSliderInstances
{
public:
	using ArrayType = TArray<FSliderInstance>;
	using ArrayTypeConst = TArray<const FSliderInstance>;

	FConstSliderInstances(TArray<FSliderInstance>& instances)
		: m_Instances(instances)
	{

	}

	FSliderInstance& operator[](int32 idx) { return m_Instances[idx]; }
	int32 Num() const { return m_Instances.Num(); }

	FORCEINLINE TArray<FSliderInstance>::RangedForIteratorType             begin() { return m_Instances.begin(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForConstIteratorType        begin() const { return const_cast<const ArrayType*>(&m_Instances)->begin(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForIteratorType             end() { return m_Instances.end(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForConstIteratorType        end() const { return const_cast<const ArrayType*>(&m_Instances)->end(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForReverseIteratorType      rbegin() { return m_Instances.rbegin(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForConstReverseIteratorType rbegin() const { return const_cast<const ArrayType*>(&m_Instances)->rbegin(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForReverseIteratorType      rend() { return m_Instances.rend(); }
	FORCEINLINE TArray<FSliderInstance>::RangedForConstReverseIteratorType rend() const { return const_cast<const ArrayType*>(&m_Instances)->rend(); }
private:
	TArray<FSliderInstance>& m_Instances;
};

UCLASS( ClassGroup=(SliderCustomization), meta=(BlueprintSpawnableComponent), Blueprintable )
class SLIDERVALUES_API USliderValuesComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class USliderValuesManager;
	static TDelegate<void(USliderValuesComponent*)> OnCreated;
	static TDelegate<void(USliderValuesComponent*)> OnDestroyed;
	static TDelegate<void(USliderValuesComponent*)> OnUpdated;

	// Data is allocated by manager for efficiency.
	struct FData
	{
		static constexpr int32 INVALID_ID = 0xffffffff;
		USliderValuesComponent* m_Component = nullptr;
		int32 m_Id = INVALID_ID;
		bool m_IsDirty : 1 = false;
	};
public:
	// Sets default values for this component's properties
	USliderValuesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateSliders();

	UFUNCTION(BlueprintCallable, meta = (RequiredAssetDataTags = "RowStructure=/Script/SliderValues.SliderDefinition"))
	void RegisterSliderDefinitions(UDataTable* sliderDefinitionsDt);

	static float GetDefaultValue(const FSliderDefinition& sliderDefinition);

	int32 GetSliderIdx(const FName& sliderId) const;
	const TArray<FSliderInstance>& GetSliderInstances() const { return SliderInstances; }
	FConstSliderInstances GetSliderInstances() { return FConstSliderInstances(SliderInstances); }
	
	float GetSliderValue(const FName& sliderId) const;
	float GetSliderValue(int32 sliderIdx) const;

	const TArray<FSliderDefinition>& GetSliderDefinitions() const { return SliderDefinitions; }
	TArray<float> GetSliderValues() const;
	const TArray<FName>& GetSliderIds() const { return SliderIds; }

	void ResetDefaultValues();
	void ResetDefaultValue(const FName& sliderId, bool sendChangedEvent = true);
	void ResetDefaultValue(int32 sliderIdx, bool sendChangedEvent = true);

	void SetSliderValue(const FName sliderId, float value, bool sendChangedEvent = true);
	void SetSliderValue(int32 sliderIdx, float value, bool sendChangedEvent = true);
	void SetSliderValues(const TMap<FName, float>& sliderIdToValues);
	void SetSliderValues(const TArray<float>& sliderValues);

	void OnValuesChanged();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type reason) override;

	void BuildSliderInstances(const TArray<FName>& sliderIds, const UDataTable& sliderDefinitionsDt);
	void BuildSliderIndices(const TArray<FName>& sliderIds);

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> SliderDefinitionsDt;

	// A Parameter can only be controlled by one, and only one slider.
	// This allows for selective updates, see UpdateModified()
	bool ExclusiveParameters;

	TArray<FName> SliderIds;
	TArray<FSliderInstance> SliderInstances;
	TArray<FSliderDefinition> SliderDefinitions;
	TMap<FName, int32> SliderIndexMap;
	UCurveTable* CompositeSliders;

	TSharedPtr<FData> ComponentData;
};
