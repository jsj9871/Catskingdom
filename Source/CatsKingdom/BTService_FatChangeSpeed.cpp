// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FatChangeSpeed.h"
#include "FatVillian.h"
#include "CKAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UBTService_FatChangeSpeed::UBTService_FatChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");

	//TVMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TVMesh"));
	//RootComponent = TVMesh;
}

// 용사 감지하면 속도 증가
void UBTService_FatChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	AFatVillian* const Fat = Cast<AFatVillian>(cont->GetPawn());
	Fat->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FString UBTService_FatChangeSpeed::GetStaticServiceDescription() const
{
	return FString();
}

void UBTService_FatChangeSpeed::OnOverlapBegin()
{
	//TVMesh->SetMaterial(0, OnMaterial);
}