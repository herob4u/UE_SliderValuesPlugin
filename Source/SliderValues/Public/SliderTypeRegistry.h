// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SliderLinkBase.h"

struct FSliderTypeInfo
{
	UClass* SliderLinkClass;
};

template<typename TField, typename TSliderLink>
struct TSliderTypeInfo
{
	using TargetFieldType = TField;
	using SliderLinkType = TSliderLink;
};

/**
 * 
 */
class SLIDERVALUES_API SliderTypeRegistry
{
public:
	template<typename TSliderLink>
	static void Register(/*eval func*/)
	{
		//static_assert(TIsDerivedFrom<TSliderLink, USliderLinkBase>::)
	}
};
