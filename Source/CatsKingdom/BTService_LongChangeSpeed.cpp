// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LongChangeSpeed.h"
#include "ThinLongVillain.h"
#include "LongAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


UBTService_LongChangeSpeed::UBTService_LongChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

// 용사 감지하면 속도 증가
void UBTService_LongChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	AThinLongVillain* const ThinLong = Cast<AThinLongVillain>(cont->GetPawn());
	ThinLong->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FString UBTService_LongChangeSpeed::GetStaticServiceDescription() const
{
	return FString();
}