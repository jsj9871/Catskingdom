// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "CKAIController.h"
#include "ThinVillain.h"
#include "CatsKingdomCharacter.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

// 악당냥이 공격
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ThinVillain = Cast<AThinVillain>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ThinVillain)
		return EBTNodeResult::Failed;

	ThinVillain->Attack();
	IsAttacking = true;
	ThinVillain->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

// 태스크 종료
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


