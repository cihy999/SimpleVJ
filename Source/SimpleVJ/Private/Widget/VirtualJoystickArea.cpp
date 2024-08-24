// Copyright Epic Games, Inc. All Rights Reserved.

// Self
#include "Widget/VirtualJoystickArea.h"
//
#include "Widget/Slate/SVirtualJoystickArea.h"

UVirtualJoystickArea::UVirtualJoystickArea(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
}

void UVirtualJoystickArea::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (!MyVirtualJoystickArea.IsValid())
    {
        return;
    }

    TAttribute<const FSlateBrush*> PreviewImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, PreviewImage, const FSlateBrush*, ConvertImage);
    MyVirtualJoystickArea->SetPreviewImage(PreviewImageBinding);
}

void UVirtualJoystickArea::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MyVirtualJoystickArea.Reset();
}

const FText UVirtualJoystickArea::GetPaletteCategory()
{
    return FText::FromString(TEXT("Input"));
}

TSharedRef<SWidget> UVirtualJoystickArea::RebuildWidget()
{
    MyVirtualJoystickArea = SNew(SVirtualJoystickArea);

    return MyVirtualJoystickArea.ToSharedRef();
}

const FSlateBrush* UVirtualJoystickArea::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
    UVirtualJoystickArea* MutableThis = const_cast<UVirtualJoystickArea*>(this);
    MutableThis->PreviewImage = InImageAsset.Get();

    return &PreviewImage;
}
