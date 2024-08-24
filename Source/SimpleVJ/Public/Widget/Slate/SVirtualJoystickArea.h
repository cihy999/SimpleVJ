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
		: _PreviewImage(FCoreStyle::Get().GetDefaultBrush())
		{}
		SLATE_ATTRIBUTE(const FSlateBrush*, PreviewImage)
	SLATE_END_ARGS()

	SVirtualJoystickArea();

	void Construct(const FArguments& InArgs);

	// ~Begin SWidget Interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// ~End SWidget Interface

protected:
	// ~Begin SLeafWidget Interface
	virtual FVector2D ComputeDesiredSize(float) const override;
	// ~End SLeafWidget Interface

// =================================================================================================================
public:
	void SetPreviewImage(TAttribute<const FSlateBrush*> InPreviewImage);
	const FSlateBrush* GetPreviewImage() const { return PreviewImageAttribute.Get(); }

protected:
	TSlateAttributeRef<const FSlateBrush*> GetPreviewImageAttribute() const { return TSlateAttributeRef<const FSlateBrush*>(SharedThis(this), PreviewImageAttribute); }

private:
	TSlateAttribute<const FSlateBrush*> PreviewImageAttribute;
};
