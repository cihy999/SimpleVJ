// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// UE
#include "CoreMinimal.h"
#include "Components/Widget.h"
// 
#include "VirtualJoystickArea.generated.h"

class SVirtualJoystickArea;

/**
 * 
 */
UCLASS()
class SIMPLEVJ_API UVirtualJoystickArea : public UWidget
{
	GENERATED_UCLASS_BODY()
	
public:
    // ~Begin UWidget Interface
    virtual void SynchronizeProperties() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    // ~End UVisual Interface

protected:
    // ~Begin UWidget Interface
    virtual TSharedRef<SWidget> RebuildWidget() override;
    // ~End UWidget Interface

protected:
    TSharedPtr<SVirtualJoystickArea> MyVirtualJoystickArea;
};
