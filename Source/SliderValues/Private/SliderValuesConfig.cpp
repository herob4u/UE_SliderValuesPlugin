// Fill out your copyright notice in the Description page of Project Settings.


#include "SliderValuesConfig.h"
#include "SliderValuesModule.h"
#include "Modules/ModuleManager.h"

const USliderValuesConfig& USliderValuesConfig::GetCurrentConfig()
{
	if (const FSliderValuesModule* module = FModuleManager::GetModulePtr<FSliderValuesModule>(TEXT("SliderValues")))
	{
		if (module->GetConfigCallback.IsBound())
		{
			if (USliderValuesConfig* config = module->GetConfigCallback.Execute())
			{
				return *config;
			}
		}
	}

	return *USliderValuesConfig::StaticClass()->GetDefaultObject<USliderValuesConfig>();
}

TArray<FName> USliderValuesConfig::GetAllMorphTargets()
{
	TArray<FName> morphTargets;
	const TArray<TObjectPtr<USkeletalMesh>>& referenceMeshes = GetCurrentConfig().MorphReferenceMeshes;

	if (referenceMeshes.Num())
	{
		morphTargets.Reserve(32 * referenceMeshes.Num());
		for (const USkeletalMesh* mesh : referenceMeshes)
		{
			for (const UMorphTarget* morphTarget : mesh->GetMorphTargets())
			{
				morphTargets.Add(morphTarget->GetFName());
			}
		}
	}

	return morphTargets;
}
