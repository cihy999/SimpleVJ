// Copyright Cindy Chen. All Rights Reserved.

// Self
#include "Widget/Slate/SVirtualJoystickArea.h"

SLATE_IMPLEMENT_WIDGET(SVirtualJoystickArea)
void SVirtualJoystickArea::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

void SVirtualJoystickArea::Construct(const FArguments& InArgs)
{

}

int32 SVirtualJoystickArea::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return LayerId;
}

FVector2D SVirtualJoystickArea::ComputeDesiredSize(float) const
{
	// FIXME: 不確定這個尺寸計算實際的影響，先給0
	return FVector2D::ZeroVector;
}
