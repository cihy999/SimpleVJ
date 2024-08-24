// Copyright Cindy Chen. All Rights Reserved.

// Self
#include "Widget/Slate/SVirtualJoystickArea.h"

SLATE_IMPLEMENT_WIDGET(SVirtualJoystickArea)
void SVirtualJoystickArea::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "PreviewImage", PreviewImageAttribute, EInvalidateWidgetReason::Paint);
}

SVirtualJoystickArea::SVirtualJoystickArea()
	: PreviewImageAttribute(*this, FCoreStyle::Get().GetDefaultBrush())
{}

void SVirtualJoystickArea::Construct(const FArguments& InArgs)
{
	SetPreviewImage(InArgs._PreviewImage);
}

int32 SVirtualJoystickArea::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateBrush* brush = PreviewImageAttribute.Get();
	if (brush && brush->GetDrawType() != ESlateBrushDrawType::NoDrawType)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			brush,
			ESlateDrawEffect::None,
			brush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
		);
	}

	return LayerId;
}

FVector2D SVirtualJoystickArea::ComputeDesiredSize(float) const
{
	// FIXME: 不確定這個尺寸計算實際的影響，先給0
	return FVector2D::ZeroVector;
}

void SVirtualJoystickArea::SetPreviewImage(TAttribute<const FSlateBrush*> InPreviewImage)
{
	PreviewImageAttribute.Assign(*this, InPreviewImage);
}
