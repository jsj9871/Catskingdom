// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FatLongVillian.h"
#include "BTTask_FatLongAttack.generated.h"

/**
 * 
 */
UCLASS()
class CATSKINGDOM_API UBTTask_FatLongAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FatLongAttack(FObjectInitializer const& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

private:
	bool montage_has_finished(AFatLongVillian* const npcBot);
};
