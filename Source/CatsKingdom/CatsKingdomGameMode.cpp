// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatsKingdomGameMode.h"
#include "CatsKingdomCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACatsKingdomGameMode::ACatsKingdomGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
