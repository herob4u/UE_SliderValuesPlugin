#include "SliderCustomizationComponent.h"

#include "SliderValuesComponent.h"
#include "SliderLinkBone.h"
#include "SliderLinkMaterial.h"
#include "SliderLinkMorpher.h"

#include "Curves/CurveFloat.h"

void USliderCustomizationComponent::Update(const USliderValuesComponent* sliderValuesComp)
{
	if (!sliderValuesComp)
	{
		return;
	}

	const TArray<FSliderInstance>& sliderInstances = sliderValuesComp->GetSliderInstances();
	for (const FSliderInstance& sliderInstance : sliderInstances)
	{
		for (const auto& sliderLink : sliderInstance.Links)
		{
			UpdateSliderLink(sliderInstance.Value, sliderLink);
		}
	}
}

void USliderCustomizationComponent::ResetValues()
{
	BoneValues.Empty(BoneValues.Num());
	MorpherValues.Empty(MorpherValues.Num());
	MaterialValues.Empty(MaterialValues.Num());
}

void USliderCustomizationComponent::UpdateSliderLink(float sliderValue, const TInstancedStruct<FSliderLinkBase>& sliderLink)
{
	const UScriptStruct* linkStruct = sliderLink.GetScriptStruct();
	if (linkStruct == FSliderLinkBone::StaticStruct())
	{
		UpdateBoneLink(sliderValue, sliderLink.Get<FSliderLinkBone>());
	}
	else if (linkStruct == FSliderLinkMaterial::StaticStruct())
	{
		UpdateMaterialLink(sliderValue, sliderLink.Get<FSliderLinkMaterial>());
	}
	else if (linkStruct == FSliderLinkMorpher::StaticStruct())
	{
		UpdateMorpherLink(sliderValue, sliderLink.Get<FSliderLinkMorpher>());
	}
}

void USliderCustomizationComponent::UpdateBoneLink(float sliderValue, const FSliderLinkBone& boneLink)
{
	FVector euler, position, scale;
	euler = position = scale = FVector::ZeroVector;

	if(boneLink.EulerStateValues)
		euler = boneLink.EulerStateValues->GetVectorValue(sliderValue);

	if(boneLink.PositionStateValues)
		position = boneLink.PositionStateValues->GetVectorValue(sliderValue);

	if(boneLink.ScaleStateValues)
		scale = boneLink.ScaleStateValues->GetVectorValue(sliderValue);

	euler.UnwindEuler();
	FQuat quat = FQuat::MakeFromEuler(euler).GetNormalized();

	FTransform tm(quat);
	tm.SetTranslation(position);
	tm.SetScale3D(scale);

	// Update the bones by superimposing the offset
	for (const FName& boneId : boneLink.LinkedBones)
	{
		if (FTransform* currTm = BoneValues.Find(boneId))
		{
			currTm->operator*(tm); // concat the transforms
		}
		else
		{
			BoneValues.Add(boneId, tm);
		}
	}
}

void USliderCustomizationComponent::UpdateMaterialLink(float sliderValue, const FSliderLinkMaterial& materialLink)
{
	checkf(false, TEXT("NOT IMPLEMENTED"));
}

void USliderCustomizationComponent::UpdateMorpherLink(float sliderValue, const FSliderLinkMorpher& morpherLink)
{
	const float value = morpherLink.StateValues->GetFloatValue(sliderValue);
	for (const FName& morpherId : morpherLink.LinkedMorphTargets)
	{
		if (float* currValue = MorpherValues.Find(morpherId))
		{
			*currValue += value;
		}
		else
		{
			MorpherValues.Add(morpherId, value);
		}
	}
}
