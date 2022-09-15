// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "CKAIController.h"
#include "LongAIController.h"
#include "CatsKingdomCharacter.h"
#include "ThinVillain.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("Turn");
}

// 용사 향해 회전
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto CatsKingdomCharacter = Cast<AThinVillain>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == CatsKingdomCharacter)
        return EBTNodeResult::Failed;

    auto Target = Cast<ACatsKingdomCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACKAIController::TargetKey));
    if (nullptr == Target)
        return EBTNodeResult::Failed;

    FVector LookVector = Target->GetActorLocation() - CatsKingdomCharacter->GetActorLocation();
    LookVector.Z = 0.0f;
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    CatsKingdomCharacter->SetActorRotation(FMath::RInterpTo(CatsKingdomCharacter->GetActorRotation(),
        TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

    return EBTNodeResult::Succeeded;
}