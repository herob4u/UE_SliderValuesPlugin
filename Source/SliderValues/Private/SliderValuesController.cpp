#include "SliderValuesController.h"

#include "SliderValuesComponent.h"
#include "SliderLinkBone.h"
#include "SliderLinkMaterial.h"
#include "SliderLinkMorpher.h"

#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"

void USliderValuesController::Update(const USliderValuesComponent* sliderValuesComp)
{
	if (!sliderValuesComp)
	{
		return;
	}

	ResetValues();

	if (bExclusiveParameters && sliderValuesComp->GetSliderInstances().Num() == PrevSliderValues.Num())
	{
		UpdateModified(sliderValuesComp);
	}
	else
	{
		UpdateAll(sliderValuesComp);
	}
}

void USliderValuesController::UpdateAll(const USliderValuesComponent* sliderValuesComp)
{
	const TArray<FSliderInstance>& sliderInstances = sliderValuesComp->GetSliderInstances();
	PrevSliderValues.Reserve(sliderInstances.Num());

	for (int32 sliderIdx = 0; sliderIdx < sliderInstances.Num(); ++sliderIdx)
	{
		const FSliderInstance& sliderInstance = sliderInstances[sliderIdx];
		for (const auto& sliderLink : sliderInstance.Links)
		{
			UpdateSliderLink(sliderInstance.Value, sliderLink);
		}

		PrevSliderValues[sliderIdx] = sliderInstance.Value;
	}
}

// Updating only the modified slider wont work, because it's possible the modified slider touched a parameter controlled by other sliders too.
// So we would fail to undo its contribution to the other values. Must do full recompute, unless we guarantee EXCLUSION on the controlled param
void USliderValuesController::UpdateModified(const USliderValuesComponent* sliderValuesComp)
{
	const TArray<FSliderInstance>& sliderInstances = sliderValuesComp->GetSliderInstances();
	check(sliderInstances.Num() == PrevSliderValues.Num());

	for (int32 sliderIdx = 0; sliderIdx < sliderInstances.Num(); ++sliderIdx)
	{
		const FSliderInstance& sliderInstance = sliderInstances[sliderIdx];
		if (PrevSliderValues[sliderIdx] != sliderInstance.Value)
		{
			for (const auto& sliderLink : sliderInstance.Links)
			{
				UpdateSliderLink(sliderInstance.Value, sliderLink);
			}

			PrevSliderValues[sliderIdx] = sliderInstance.Value;
		}
	}
}

void USliderValuesController::ApplyMorphers(const TMap<FName, float>& morpherValues, USkeletalMeshComponent* skMeshComponent)
{
	if (!skMeshComponent)
		return;

	skMeshComponent->SetScalarParameterValueOnMaterials("", 1.0f);
}

void USliderValuesController::ApplyMaterials(const TMap<FName, float>& materialValues, USkeletalMeshComponent* skMeshComponent)
{
	if (!skMeshComponent)
		return;

	for (auto pair : materialValues)
	{
		skMeshComponent->SetScalarParameterValueOnMaterials(pair.Key, pair.Value);
	}
}

void USliderValuesController::ApplyMaterials(const TMap<FName, float>& materialValues, UMaterialInstanceDynamic* dynamicMaterialInstance)
{
	if (!dynamicMaterialInstance)
		return;

	for (auto pair : materialValues)
	{
		dynamicMaterialInstance->SetScalarParameterValue(pair.Key, pair.Value);
	}
}

void USliderValuesController::ResetValues()
{
	BoneValues.Empty(BoneValues.Num());
	MorpherValues.Empty(MorpherValues.Num());
	MaterialScalarValues.Empty(MaterialScalarValues.Num());
	MaterialColorValues.Empty(MaterialColorValues.Num());
}

void USliderValuesController::UpdateSliderLink(float sliderValue, const TInstancedStruct<FSliderLinkBase>& sliderLink)
{
	const UScriptStruct* linkStruct = sliderLink.GetScriptStruct();
	if (linkStruct == FSliderLinkBone::StaticStruct())
	{
		UpdateBoneLink(sliderValue, sliderLink.Get<FSliderLinkBone>());
	}
	else if (linkStruct == FSliderLinkMaterialScalar::StaticStruct())
	{
		UpdateMaterialLink(sliderValue, sliderLink.Get<FSliderLinkMaterialScalar>());
	}
	else if (linkStruct == FSliderLinkMaterialColor::StaticStruct())
	{
		UpdateMaterialLink(sliderValue, sliderLink.Get<FSliderLinkMaterialColor>());
	}
	else if (linkStruct == FSliderLinkMorpher::StaticStruct())
	{
		UpdateMorpherLink(sliderValue, sliderLink.Get<FSliderLinkMorpher>());
	}
}

void USliderValuesController::UpdateBoneLink(float sliderValue, const FSliderLinkBone& boneLink)
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
			if(!bExclusiveParameters)
				currTm->operator*(tm); // concat the transforms
		}
		else
		{
			BoneValues.Add(boneId, tm);
		}
	}
}

void USliderValuesController::UpdateMaterialLink(float sliderValue, const FSliderLinkMaterialScalar& materialLink)
{
	const float value = materialLink.StateValues->GetFloatValue(sliderValue);
	for (const FName& scalarParam : materialLink.LinkedParams)
	{
		if (float* currValue = MaterialScalarValues.Find(scalarParam))
		{
			if(!bExclusiveParameters)
				*currValue += value;
		}
		else
		{
			MaterialScalarValues.Add(scalarParam, value);
		}
	}
}

void USliderValuesController::UpdateMaterialLink(float sliderValue, const FSliderLinkMaterialColor& materialLink)
{
	const FLinearColor value = materialLink.StateValues->GetLinearColorValue(sliderValue);
	for (const FName& scalarParam : materialLink.LinkedParams)
	{
		if (FLinearColor* currValue = MaterialColorValues.Find(scalarParam))
		{
			if(!bExclusiveParameters)
				*currValue += value;
		}
		else
		{
			MaterialColorValues.Add(scalarParam, value);
		}
	}
}

void USliderValuesController::UpdateMorpherLink(float sliderValue, const FSliderLinkMorpher& morpherLink)
{
	const float value = morpherLink.StateValues->GetFloatValue(sliderValue);
	for (const FName& morpherId : morpherLink.LinkedMorphTargets)
	{
		if (float* currValue = MorpherValues.Find(morpherId))
		{
			if(!bExclusiveParameters)
				*currValue += value;
		}
		else
		{
			MorpherValues.Add(morpherId, value);
		}
	}
}
