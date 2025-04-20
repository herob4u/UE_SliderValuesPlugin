#pragma once

#include "Components/ActorComponent.h"
#include "SliderDefinitions.h"
#include "SliderValuesController.generated.h"

class USliderValuesController;
class USliderValuesComponent;
struct FSliderLinkBone;
struct FSliderLinkMaterialScalar;
struct FSliderLinkMaterialColor;
struct FSliderLinkMorpher;

class ISliderCustomizationVisitor
{
public:
	virtual void Visit(const USliderValuesController* controller) = 0;
};

// Runtime that takes the slider values, and evaluates them with values that will be applied to a mesh/actor
// Subclass to evaluate new slider link types and apply them
// This class is not responsible for applying the effects, only a store of final values
UCLASS(ClassGroup = (SliderCustomization), meta = (BlueprintSpawnableComponent))
class SLIDERVALUES_API USliderValuesController : public UActorComponent
{
	GENERATED_BODY()
public:
	void Update(const USliderValuesComponent* sliderValuesComp);
	void UpdateAll(const USliderValuesComponent* sliderValuesComp);
	void UpdateModified(const USliderValuesComponent* sliderValuesComp);

	virtual void Accept(ISliderCustomizationVisitor* visitor)
	{
		if(visitor)
			visitor->Visit(this);
	}

	static void ApplyMorphers(const TMap<FName, float>& morpherValues, USkeletalMeshComponent* skMeshComponent);
	static void ApplyMaterials(const TMap<FName, float>& morpherValues, USkeletalMeshComponent* skMeshComponent);
	static void ApplyMaterials(const TMap<FName, float>& MaterialScalarValues, UMaterialInstanceDynamic* dynamicMaterialInstance);

	const TMap<FName, FTransform>& GetBoneValues() const { return BoneValues; }
	const TMap<FName, float>& GetMorpherValues() const { return MorpherValues; }
	const TMap<FName, float>& GetMaterialScalarValues() const { return MaterialScalarValues; }
	const TMap<FName, FLinearColor> GetMaterialColorValues() const { return MaterialColorValues; }
	// GetCustomValues...

	// A Parameter can only be controlled by one, and only one slider.
	// This allows for selective updates, see UpdateModified()
	bool bExclusiveParameters;
protected:
	void ResetValues();
	virtual void UpdateSliderLink(float sliderValue, const TInstancedStruct<FSliderLinkBase>& link);
	void UpdateBoneLink(float sliderValue, const FSliderLinkBone& boneLink);
	void UpdateMaterialLink(float sliderValue, const FSliderLinkMaterialScalar& materialLink);
	void UpdateMaterialLink(float sliderValue, const FSliderLinkMaterialColor& materialLink);
	void UpdateMorpherLink(float sliderValue, const FSliderLinkMorpher& morpherLink);

	// UpdateCustomLink

	TMap<FName, FTransform> BoneValues;
	TMap<FName, float> MorpherValues;
	TMap<FName, float> MaterialScalarValues;
	TMap<FName, FLinearColor> MaterialColorValues;

	// Want to cache previous material values because these are possible expensive to update
	TMap<FName, float> PrevMaterialScalarValues;
	TMap<FName, FLinearColor> PrevMaterialColorValues;

	TArray<float> PrevSliderValues;

	// Need a Map<LinkStructType, Updater> to make the updates fully data driven
	// CustomValues
};