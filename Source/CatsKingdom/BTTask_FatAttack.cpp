// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FatAttack.h"
#include "CKAIController.h"
#include "FatVillian.h"
#include "CatsKingdomCharacter.h"


UBTTask_FatAttack::UBTTask_FatAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

// �Ǵ���� ����
EBTNodeResult::Type UBTTask_FatAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto FatVillian = Cast<AFatVillian>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == FatVillian)
		return EBTNodeResult::Failed;

	FatVillian->Attack();
	IsAttacking = true;
	FatVillian->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

// �½�ũ ����
void UBTTask_FatAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


