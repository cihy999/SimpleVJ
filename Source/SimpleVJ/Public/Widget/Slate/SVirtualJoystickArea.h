// Copyright Cindy Chen. All Rights Reserved.

#pragma once

// UE
#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 自製虛擬搖桿
 */
class SIMPLEVJ_API SVirtualJoystickArea : public SLeafWidget
{
	SLATE_DECLARE_WIDGET(SVirtualJoystickArea, SLeafWidget)

public:
	SLATE_BEGIN_ARGS(SVirtualJoystickArea)
		{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// ~Begin SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// ~End SWidget interface

protected:
	// ~Begin SLeafWidget interface
	virtual FVector2D ComputeDesiredSize(float) const override;
	// ~End SLeafWidget interface
};
