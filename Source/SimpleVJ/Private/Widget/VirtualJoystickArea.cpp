// Copyright Epic Games, Inc. All Rights Reserved.

// Self
#include "Widget/VirtualJoystickArea.h"
//
#include "Widget/Slate/SVirtualJoystickArea.h"

UVirtualJoystickArea::UVirtualJoystickArea(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{}

void UVirtualJoystickArea::SynchronizeProperties()
{
    Super::SynchronizeProperties();
}

void UVirtualJoystickArea::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MyVirtualJoystickArea.Reset();
}

TSharedRef<SWidget> UVirtualJoystickArea::RebuildWidget()
{
    MyVirtualJoystickArea = SNew(SVirtualJoystickArea);

    return MyVirtualJoystickArea.ToSharedRef();
}
