// Copyright Epic Games, Inc. All Rights Reserved.

#include "SliderValuesModule.h"
#include "SliderLinkBone.h"
#include "SliderLinkMaterial.h"
#include "SliderDefinitions.h"

#define LOCTEXT_NAMESPACE "FSliderValuesModule"

void FSliderValuesModule::StartupModule()
{
}

void FSliderValuesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSliderValuesModule, SliderValues)