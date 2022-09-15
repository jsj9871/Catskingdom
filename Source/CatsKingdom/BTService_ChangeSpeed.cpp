// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "ThinVillain.h"
#include "CKAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");

	//TVMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TVMesh"));
	//RootComponent = TVMesh;
}

// 용사 감지하면 속도 증가
void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	AThinVillain* const Thin = Cast<AThinVillain>(cont->GetPawn());
	Thin->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FString UBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString();
}

void UBTService_ChangeSpeed::OnOverlapBegin()
{
		//TVMesh->SetMaterial(0, OnMaterial);
}