// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleVJGameMode.h"
#include "SimpleVJCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASimpleVJGameMode::ASimpleVJGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
