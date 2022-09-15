 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CatsKingdom.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CKAIController.generated.h"

/**
 * 
 */

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class CATSKINGDOM_API ACKAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    //ACKAIController();
    virtual void OnPossess(APawn* InPawn) override;
    ACKAIController(FObjectInitializer const& object_intializer = FObjectInitializer::Get());
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds)override;

    static const FName HomePosKey;
    static const FName PatrolPosKey;
    static const FName TargetKey;

private:
    UPROPERTY()
        class UBehaviorTree* BTAsset;

    UPROPERTY()
        class UBlackboardData* BBAsset;
};
