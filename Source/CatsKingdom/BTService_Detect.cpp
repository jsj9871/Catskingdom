// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "CKAIController.h"
#include "CatsKingdomCharacter.h"
#include "ThinVillain.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// 탐지 영역 내의 캐릭터 감지
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 2000.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// 용사 플레이어 감지
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ACatsKingdomCharacter* CatsKingdomCharacter = Cast<ACatsKingdomCharacter>(OverlapResult.GetActor());
			if (CatsKingdomCharacter && CatsKingdomCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACKAIController::TargetKey, CatsKingdomCharacter);
				/*DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, CatsKingdomCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(),
					CatsKingdomCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);*/
				return;
			}
		}
	}

	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}

