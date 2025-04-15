// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliderLinkBone.h"

/**
 * 
 */
class SLIDERVALUES_API SliderRuntime
{
public:
	SliderRuntime();
	~SliderRuntime();

	void MarkDirty(){
		//USliderLinkBone* link = nullptr;
		//SliderEvaluator<USliderLinkBone>::Evaluate(.5f, *link);
	}
	void ResetDirty(){}
private:
	bool m_NeedsUpdate;
};
