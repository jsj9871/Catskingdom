// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatsKingdomGameMode.generated.h"

UCLASS(minimalapi)
class ACatsKingdomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACatsKingdomGameMode();

	UPROPERTY()
		int TotalAnemy = 19;
};



