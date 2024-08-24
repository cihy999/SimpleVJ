// Copyright Epic Games, Inc. All Rights Reserved.

// Self
#include "Widget/VirtualJoystickArea.h"
//
#include "Widget/Slate/SVirtualJoystickArea.h"

UVirtualJoystickArea::UVirtualJoystickArea(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
    InteractionColor = FLinearColor(1, 1, 1, 0.2f);
}

FLinearColor UVirtualJoystickArea::GetInteractionColor() const
{
    return InteractionColor;
}

void UVirtualJoystickArea::SetInteractionColor(FLinearColor InInteractionColor)
{
    InteractionColor = InInteractionColor;
    if (MyVirtualJoystickArea.IsValid())
    {
        MyVirtualJoystickArea->SetInteractionColor(InInteractionColor);
    }
}

void UVirtualJoystickArea::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (!MyVirtualJoystickArea.IsValid())
    {
        return;
    }

    TAttribute<FSlateColor> InteractionColorBinding = OPTIONAL_BINDING_CONVERT(FLinearColor, InteractionColor, FSlateColor, ConvertLinearColorToSlateColor);
    TAttribute<const FSlateBrush*> PreviewImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, PreviewImage, const FSlateBrush*, ConvertImage);
    MyVirtualJoystickArea->SetInteractionColor(InteractionColorBinding);
    MyVirtualJoystickArea->SetPreviewImage(PreviewImageBinding);
}

void UVirtualJoystickArea::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MyVirtualJoystickArea.Reset();
}

#if WITH_EDITOR
const FText UVirtualJoystickArea::GetPaletteCategory()
{
    return FText::FromString(TEXT("Input"));
}
#endif

TSharedRef<SWidget> UVirtualJoystickArea::RebuildWidget()
{
    MyVirtualJoystickArea = SNew(SVirtualJoystickArea);

    if (!IsDesignTime())
    {
        InteractionColor = FLinearColor(1, 1, 1, 0.f);
    }

    return MyVirtualJoystickArea.ToSharedRef();
}

const FSlateBrush* UVirtualJoystickArea::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
    UVirtualJoystickArea* MutableThis = const_cast<UVirtualJoystickArea*>(this);
    MutableThis->PreviewImage = InImageAsset.Get();

    return &PreviewImage;
}
