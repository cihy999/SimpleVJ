// Copyright Cindy Chen. All Rights Reserved.

// Self
#include "Widget/Slate/SVirtualJoystickArea.h"

SLATE_IMPLEMENT_WIDGET(SVirtualJoystickArea)
void SVirtualJoystickArea::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "PreviewImage", PreviewImageAttribute, EInvalidateWidgetReason::Paint);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "InteractionColor", InteractionColorAttribute, EInvalidateWidgetReason::Paint);
}

SVirtualJoystickArea::SVirtualJoystickArea()
	: PreviewImageAttribute(*this, FCoreStyle::Get().GetDefaultBrush())
	, InteractionColorAttribute(*this, FLinearColor::White)
{}

void SVirtualJoystickArea::Construct(const FArguments& InArgs)
{
	SetPreviewImage(InArgs._PreviewImage);
	SetInteractionColor(InArgs._InteractionColor);

	OnTickEvent = InArgs._OnTickEvent;
	OnTouchStartedEvent = InArgs._OnTouchStartedEvent;
	OnTouchMovedEvent = InArgs._OnTouchMovedEvent;
	OnTouchEndedEvent = InArgs._OnTouchEndedEvent;
}

void SVirtualJoystickArea::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	OnTickEvent.ExecuteIfBound(AllottedGeometry, InDeltaTime);
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
			brush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * InteractionColorAttribute.Get().GetColor(InWidgetStyle)
		);
	}

	return LayerId;
}

FReply SVirtualJoystickArea::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	if (OnTouchStartedEvent.IsBound())
	{
		return OnTouchStartedEvent.Execute(MyGeometry, InTouchEvent);
	}
	return FReply::Unhandled();
}

FReply SVirtualJoystickArea::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	if (OnTouchMovedEvent.IsBound())
	{
		return OnTouchMovedEvent.Execute(MyGeometry, InTouchEvent);
	}
	return FReply::Unhandled();
}

FReply SVirtualJoystickArea::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	if (OnTouchEndedEvent.IsBound())
	{
		return OnTouchEndedEvent.Execute(MyGeometry, InTouchEvent);
	}
	return FReply::Unhandled();
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

void SVirtualJoystickArea::SetInteractionColor(TAttribute<FSlateColor> InColorAndOpacity)
{
	InteractionColorAttribute.Assign(*this, InColorAndOpacity);
}
