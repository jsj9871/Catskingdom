// Fill out your copyright notice in the Description page of Project Settings.


#include "CKAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "CatsKingdomCharacter.h"
#include "Engine.h"

// 키 값 지정
const FName ACKAIController::HomePosKey(TEXT("HomePos"));
const FName ACKAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACKAIController::TargetKey(TEXT("Target"));

// 비헤이비어 트리 구동
ACKAIController::ACKAIController(FObjectInitializer const& object_intializer)
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_CKCharacter.BB_CKCharacter"));
    if (BBObject.Succeeded())
    {
        BBAsset = BBObject.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_CKCharacter.BT_CKCharacter"));
    if (BTObject.Succeeded())
    {
        BTAsset = BTObject.Object;
    }
}

void ACKAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ACKAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (UseBlackboard(BBAsset, Blackboard))
    {
        Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
    }
}

void ACKAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

/*FRotator ACKAIController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.f, 0.f, 0.f);
    }

    return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}*/

