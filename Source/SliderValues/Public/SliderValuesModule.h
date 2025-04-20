// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSliderValuesModule : public IModuleInterface
{
public:
#if WITH_EDITORONLY_DATA
	static TDelegate<void(TArray<USkeletalMesh*> outReferenceMeshes)> GetReferenceMeshesCallback;
#endif

	TDelegate<class USliderValuesConfig* ()> GetConfigCallback;

	//void RegisterSliderLink(const UScriptStruct* sliderLinkStruct, evalFunc);
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
