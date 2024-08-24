// Copyright Cindy Chen. All Rights Reserved.

#pragma once

// UE
#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

DECLARE_DELEGATE_TwoParams(FVirtualJoystickTickDelegate, const FGeometry&, float);
DECLARE_DELEGATE_RetVal_TwoParams(FReply, FVirtualJoystickTouchDelegate, const FGeometry&, const FPointerEvent&);

/**
 * 自製虛擬搖桿
 */
class SIMPLEVJ_API SVirtualJoystickArea : public SLeafWidget
{
	SLATE_DECLARE_WIDGET(SVirtualJoystickArea, SLeafWidget)

public:
	SLATE_BEGIN_ARGS(SVirtualJoystickArea)
		: _PreviewImage(FCoreStyle::Get().GetDefaultBrush())
		, _InteractionColor(FLinearColor::White)
		{}
		SLATE_ATTRIBUTE(const FSlateBrush*, PreviewImage)
		SLATE_ATTRIBUTE(FSlateColor, InteractionColor)
		SLATE_EVENT(FVirtualJoystickTickDelegate, OnTickEvent)
		SLATE_EVENT(FVirtualJoystickTouchDelegate, OnTouchStartedEvent)
		SLATE_EVENT(FVirtualJoystickTouchDelegate, OnTouchMovedEvent)
		SLATE_EVENT(FVirtualJoystickTouchDelegate, OnTouchEndedEvent)
	SLATE_END_ARGS()

	SVirtualJoystickArea();

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

	// ~Begin SWidget Interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// ~End SWidget Interface

	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent);
	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent);
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent);

protected:
	// ~Begin SLeafWidget Interface
	virtual FVector2D ComputeDesiredSize(float) const override;
	// ~End SLeafWidget Interface

// =================================================================================================================
public:
	void SetPreviewImage(TAttribute<const FSlateBrush*> InPreviewImage);
	const FSlateBrush* GetPreviewImage() const { return PreviewImageAttribute.Get(); }

	void SetInteractionColor(TAttribute<FSlateColor> InColorAndOpacity);
	FSlateColor GetInteractionColor() const { return InteractionColorAttribute.Get(); }

protected:
	TSlateAttributeRef<const FSlateBrush*> GetPreviewImageAttribute() const { return TSlateAttributeRef<const FSlateBrush*>(SharedThis(this), PreviewImageAttribute); }
	TSlateAttributeRef<FSlateColor> GetInteractionColorAttribute() const { return TSlateAttributeRef<FSlateColor>(SharedThis(this), InteractionColorAttribute); }

private:
	TSlateAttribute<const FSlateBrush*> PreviewImageAttribute;
	TSlateAttribute<FSlateColor> InteractionColorAttribute;

	FVirtualJoystickTickDelegate OnTickEvent;
	FVirtualJoystickTouchDelegate OnTouchStartedEvent;
	FVirtualJoystickTouchDelegate OnTouchMovedEvent;
	FVirtualJoystickTouchDelegate OnTouchEndedEvent;
};
