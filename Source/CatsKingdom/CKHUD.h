// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CKHUD.generated.h"

/**
 *
 */
UCLASS()
class CATSKINGDOM_API ACKHUD : public AHUD
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UTexture2D* CrosshairTexture;

public:
	virtual void DrawHUD() override;
};
