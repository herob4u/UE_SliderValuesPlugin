#pragma once

#include "Components/ActorComponent.h"
#include "SliderDefinitions.h"
#include "SliderCustomizationComponent.generated.h"

class USliderCustomizationComponent;
class USliderValuesComponent;
struct FSliderLinkBone;
struct FSliderLinkMaterial;
struct FSliderLinkMorpher;

class ISliderCustomizationVisitor
{
public:
	virtual void Visit(const USliderCustomizationComponent* sliderCustomizationComp) = 0;
};

// Runtime that takes the slider values, and evaluates them with values that will be applied to a mesh/actor
// Subclass to evaluate new slider link types and apply them
// This class is not responsible for applying the effects, only a store of final values
UCLASS()
class SLIDERVALUES_API USliderCustomizationComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	void Update(const USliderValuesComponent* sliderValuesComp);

	virtual void Accept(ISliderCustomizationVisitor* visitor)
	{
		if(visitor)
			visitor->Visit(this);
	}

	const TMap<FName, FTransform>& GetBoneValues() const { return BoneValues; }
	const TMap<FName, float>& GetMorpherValues() const { return MorpherValues; }
	const TMap<FName, float>& GetMaterialValues() const { return MaterialValues; }

	// GetCustomValues...
protected:
	void ResetValues();
	virtual void UpdateSliderLink(float sliderValue, const TInstancedStruct<FSliderLinkBase>& link);
	void UpdateBoneLink(float sliderValue, const FSliderLinkBone& boneLink);
	void UpdateMaterialLink(float sliderValue, const FSliderLinkMaterial& materialLink);
	void UpdateMorpherLink(float sliderValue, const FSliderLinkMorpher& morpherLink);

	// UpdateCustomLink

	TMap<FName, FTransform> BoneValues;
	TMap<FName, float> MorpherValues;
	TMap<FName, float> MaterialValues;

	// Need a Map<LinkStructType, Updater> to make the updates fully data driven
	// CustomValues
};