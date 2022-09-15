// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GKAttack.h"
#include "CKAIController.h"
#include "GateKeeper.h"
#include "CatsKingdomCharacter.h"


UBTTask_GKAttack::UBTTask_GKAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

// 악당냥이 공격
EBTNodeResult::Type UBTTask_GKAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto GateKeeper = Cast<AGateKeeper>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == GateKeeper)
		return EBTNodeResult::Failed;

	GateKeeper->Attack();
	IsAttacking = true;
	GateKeeper->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

// 태스크 종료
void UBTTask_GKAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


