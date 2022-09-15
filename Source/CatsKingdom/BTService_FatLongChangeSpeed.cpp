// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FatLongChangeSpeed.h"
#include "FatLongVillian.h"
#include "LongAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UBTService_FatLongChangeSpeed::UBTService_FatLongChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");

	//TVMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TVMesh"));
	//RootComponent = TVMesh;
}

// 용사 감지하면 속도 증가
void UBTService_FatLongChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	AFatLongVillian* const FatLong = Cast<AFatLongVillian>(cont->GetPawn());
	FatLong->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FString UBTService_FatLongChangeSpeed::GetStaticServiceDescription() const
{
	return FString();
}

void UBTService_FatLongChangeSpeed::OnOverlapBegin()
{
	//TVMesh->SetMaterial(0, OnMaterial);
}