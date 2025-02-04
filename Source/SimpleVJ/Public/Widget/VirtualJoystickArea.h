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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    FSlateBrush PreviewImage;

    UPROPERTY()
    FGetSlateBrush PreviewImageDelegate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetInteractionColor", Category = "Appearance", meta = (sRGB = "true"))
    FLinearColor InteractionColor;

    /** A bindable delegate for the BrushColor. */
    UPROPERTY()
    FGetLinearColor InteractionColorDelegate;

public:
    FLinearColor GetInteractionColor() const;

    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetInteractionColor(FLinearColor InInteractionColor);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void InitializeVirtualJoystick(UWidget* Thumb, FVector2D InConstrainedLengths);
	
public:
    // ~Begin UWidget Interface
    virtual void SynchronizeProperties() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    // ~End UWidget Interface

#if WITH_EDITOR
    // ~Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
    // ~End UWidget Interface
#endif

protected:
    // ~Begin UWidget Interface
    virtual TSharedRef<SWidget> RebuildWidget() override;
    // ~End UWidget Interface

    // 仿製Border
    /** Translates the bound brush data and assigns it to the cached brush used by this widget. */
    const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

private:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
    FReply NativeOnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent);
    FReply NativeOnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent);
    FReply NativeOnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent);

protected:
    TSharedPtr<SVirtualJoystickArea> MyVirtualJoystickArea;

// 模擬搖桿 =========================================================================================================
private:
    void ComputeThumbPosition(const FVector2D& MyLocalPosition);

private:
    TWeakObjectPtr<UWidget> ThumbWidget;
    FVector2D ConstrainedLengths = FVector2D::UnitVector;

    int32 CapturedPointerIndex = INDEX_NONE;
    FVector2D TouchLocalPosition = FVector2D::ZeroVector;
    FVector2D ThumbOffset = FVector2D::ZeroVector;

    bool bSendOneMoreEvent = false;
};
