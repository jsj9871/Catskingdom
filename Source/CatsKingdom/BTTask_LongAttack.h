// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ThinLongVillain.h"
#include "BTTask_LongAttack.generated.h"

/**
 * 
 */
UCLASS()
class CATSKINGDOM_API UBTTask_LongAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_LongAttack(FObjectInitializer const& object_intializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

private:
	bool montage_has_finished(AThinLongVillain* const npcBot);
};
