// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FatLongAttack.h"
#include "LongAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "CatsKingdomCharacter.h"
#include "FatLongVillian.h"
#include "FatAnimInstance.h"
#include "CombatInterface.h"

UBTTask_FatLongAttack::UBTTask_FatLongAttack(FObjectInitializer const& object_intializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_FatLongAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAIController* const cont = owner_comp.GetAIOwner();
	AFatLongVillian* const npcBot = Cast<AFatLongVillian>(cont->GetPawn());
	if (ICombatInterface* const icombat = Cast<ICombatInterface>(npcBot))
	{
		//icombat->melee_attack();
		if (montage_has_finished(npcBot))
		{
			icombat->Execute_melee_attack(npcBot);
		}
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_FatLongAttack::montage_has_finished(AFatLongVillian* const npcBot)
{
	return npcBot->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npcBot->get_montage());
}
