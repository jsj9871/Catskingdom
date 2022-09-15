// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_isPlayerInAttackRange.h"
#include "ThinLongVillain.h"
//#include "FatLongVillian.h"
#include "LongAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


UBTService_isPlayerInAttackRange::UBTService_isPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Attack Range");
}

void UBTService_isPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);

	ALongAIController* const cont = Cast<ALongAIController>(owner_comp.GetAIOwner());
	AThinLongVillain* const npcBot = Cast<AThinLongVillain>(cont->GetPawn());
	//AFatLongVillian* const npcBot2 = Cast<AFatLongVillian>(cont->GetPawn());

	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	cont->get_blackboard()->SetValueAsBool(bb_keys::player_is_in_AttackRange, npcBot->GetDistanceTo(player) <= Attack_range);
	//cont->get_blackboard()->SetValueAsBool(bb_keys::player_is_in_AttackRange, npcBot2->GetDistanceTo(player) <= Attack_range);
}
