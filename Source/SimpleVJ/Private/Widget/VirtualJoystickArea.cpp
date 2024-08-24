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

void UVirtualJoystickArea::InitializeVirtualJoystick(UWidget* Thumb, FVector2D InConstrainedLengths)
{
    if (IsValid(Thumb))
    {
        ThumbWidget = Thumb;
        ConstrainedLengths = InConstrainedLengths;
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
    MyVirtualJoystickArea = SNew(SVirtualJoystickArea)
        .OnTickEvent_UObject(this, &ThisClass::NativeTick)
        .OnTouchStartedEvent_UObject(this, &ThisClass::NativeOnTouchStarted)
        .OnTouchMovedEvent_UObject(this, &ThisClass::NativeOnTouchMoved)
        .OnTouchEndedEvent_UObject(this, &ThisClass::NativeOnTouchEnded);

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

void UVirtualJoystickArea::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    if (CapturedPointerIndex == INDEX_NONE && !bSendOneMoreEvent)
    {
        return;
    }

    bSendOneMoreEvent = false;
    FVector2D MyLocalPos = MyGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));
    ComputeThumbPosition(MyLocalPos);

    // Get the corrected thumb offset scale (now allows ellipse instead of assuming square)
    FVector2D ThumbScaledOffset = FVector2D(ThumbOffset.X / ConstrainedLengths.X, ThumbOffset.Y * 2.0f / ConstrainedLengths.Y);
    float ThumbSquareSum = ThumbScaledOffset.X * ThumbScaledOffset.X + ThumbScaledOffset.Y * ThumbScaledOffset.Y;
    float ThumbMagnitude = FMath::Sqrt(ThumbSquareSum);
    FVector2D ThumbNormalized = FVector2D(0.f, 0.f);
    if (ThumbSquareSum > SMALL_NUMBER)
    {
        const float Scale = 1.0f / ThumbMagnitude;
        ThumbNormalized = FVector2D(ThumbScaledOffset.X * Scale, ThumbScaledOffset.Y * Scale);
    }

    // Find the scale to apply to ThumbNormalized vector to project onto unit square
    float ToSquareScale = fabs(ThumbNormalized.Y) > fabs(ThumbNormalized.X) 
        ? FMath::Sqrt((ThumbNormalized.X * ThumbNormalized.X) / (ThumbNormalized.Y * ThumbNormalized.Y) + 1.0f)
        : ThumbNormalized.X == 0.0f 
        ? 1.0f 
        : FMath::Sqrt((ThumbNormalized.Y * ThumbNormalized.Y) / (ThumbNormalized.X * ThumbNormalized.X) + 1.0f);

    // Apply proportional offset corrected for projection to unit square
    FVector2D NormalizedOffset = ThumbNormalized * ThumbMagnitude * ToSquareScale;

    UE_LOG(LogTemp, Warning, TEXT("ThumbNormalized = %s"), *ThumbNormalized.ToString());
    UE_LOG(LogTemp, Warning, TEXT("NormalizedOffset = %s"), *NormalizedOffset.ToString());

    // now pass the fake joystick events to the game
    const FGamepadKeyNames::Type XAxis = FGamepadKeyNames::LeftAnalogX;
    const FGamepadKeyNames::Type YAxis = FGamepadKeyNames::LeftAnalogY;

    FPlatformUserId UserId = GetOwningLocalPlayer() ? GetOwningLocalPlayer()->GetPlatformUserId() : FSlateApplicationBase::SlateAppPrimaryPlatformUser;
    FInputDeviceId DeviceId = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(UserId);

    FSlateApplication::Get().OnControllerAnalog(XAxis, UserId, DeviceId, NormalizedOffset.X);
    FSlateApplication::Get().OnControllerAnalog(YAxis, UserId, DeviceId, -NormalizedOffset.Y);
}

FReply UVirtualJoystickArea::NativeOnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
    if (!MyVirtualJoystickArea.IsValid())
    {
        return FReply::Unhandled();
    }

    // skip controls already in use
    if (CapturedPointerIndex != INDEX_NONE)
    {
        return FReply::Unhandled();
    }

    CapturedPointerIndex = InTouchEvent.GetPointerIndex();
    TouchLocalPosition = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());
    return FReply::Handled().CaptureMouse(MyVirtualJoystickArea.ToSharedRef());
}

FReply UVirtualJoystickArea::NativeOnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
    // skip controls already in use
    if (CapturedPointerIndex != InTouchEvent.GetPointerIndex())
    {
        return FReply::Unhandled();
    }

    TouchLocalPosition = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());
    return FReply::Handled();
}

FReply UVirtualJoystickArea::NativeOnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
    // skip controls already in use
    if (CapturedPointerIndex != InTouchEvent.GetPointerIndex())
    {
        return FReply::Unhandled();
    }

    CapturedPointerIndex = INDEX_NONE;
    bSendOneMoreEvent = true;

    return FReply::Handled().ReleaseMouseCapture();
}

void UVirtualJoystickArea::ComputeThumbPosition(const FVector2D& MyLocalPosition)
{
    FVector2D ConstrainedOffset = FVector2D::ZeroVector;

    if (!ThumbWidget.IsValid() || CapturedPointerIndex == INDEX_NONE)
    {
        ThumbOffset = ConstrainedOffset;
        return;
    }

    FVector2D Center = ThumbWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f));
    FVector2D Offset = MyLocalPosition + TouchLocalPosition - Center;
    if (!Offset.IsNearlyZero())
    {
        // clamp to the ellipse of the stick (snaps to the visual size, so, the art should go all the way to the edge of the texture)
        float DistanceToTouchSqr = Offset.SizeSquared();
        float Angle = FMath::Atan2(Offset.Y, Offset.X);

        // length along line to ellipse: L = 1.0 / sqrt(((sin(T)/Rx)^2 + (cos(T)/Ry)^2))
        float CosAngle = FMath::Cos(Angle);
        float SinAngle = FMath::Sin(Angle);
        float XTerm = CosAngle / (ConstrainedLengths.X);
        float YTerm = SinAngle / (ConstrainedLengths.Y);
        float XYTermSqr = XTerm * XTerm + YTerm * YTerm;
        float DistanceToEdgeSqr = 1.0f / XYTermSqr;

        // only clamp 
        if (DistanceToTouchSqr > DistanceToEdgeSqr)
        {
            float DistanceToEdge = FMath::InvSqrt(XYTermSqr);
            ConstrainedOffset = FVector2D(DistanceToEdge * CosAngle, DistanceToEdge * SinAngle);
        }
        else
        {
            ConstrainedOffset = Offset;
        }
    }

    ThumbOffset = ConstrainedOffset;
}
