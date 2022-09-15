// Fill out your copyright notice in the Description page of Project Settings.


#include "ThinLongVillain.h"
#include "ThinAnimInstance.h"
#include "FatAnimInstance.h"
#include "MidBossAnimInstance.h"
#include "FinalBossAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "LongAIController.h"
//#include "CKCharacterWidget.h"
#include "CatsKingdomCharacter.h"
#include "CKProjectile.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "Engine/World.h"

AThinLongVillain::AThinLongVillain()
{
	MyHP = 0.f;
	MyMaxHP = 200.0f;
	MyHP = MyMaxHP;

	IsAttacking = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CKCharacter"));
}


int AThinLongVillain::melee_attack_Implementation()
{
	if (montage)
	{
		PlayAnimMontage(montage);
		IsAttacking = true;
		
		if (Name == "ThinLong")
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(120.0f, 30.0f, 10.0f));
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->SpawnActor<ACKProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
		}

		if (Name == "FatLong")
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(120.0f, 30.0f, 10.0f));
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->SpawnActor<ACKProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
		}

		if (Name == "MidBoss")
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(130.0f, 30.0f, 10.0f));
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->SpawnActor<ACKProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
		}

		if (Name == "FinalBoss")
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(140.0f, 30.0f, 10.0f));
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->SpawnActor<ACKProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
		}
		
	}
	return 0;
}

void AThinLongVillain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TVAnim = Cast<UThinAnimInstance>(GetMesh()->GetAnimInstance());
	FVAnim = Cast<UFatAnimInstance>(GetMesh()->GetAnimInstance());
	MBAnim = Cast<UMidBossAnimInstance>(GetMesh()->GetAnimInstance());
	FBAnim = Cast<UFinalBossAnimInstance>(GetMesh()->GetAnimInstance());
	//CKCHECK(nullptr != AnimInstance);

	//TVAnim->OnMontageEnded.AddDynamic(this, &AThinVillain::OnAttackMontageEnded);

	//TVAnim->OnAttackHitCheck.AddUObject(this, &AThinVillain::AttackCheck);
}

// 데미지 처리
float AThinLongVillain::TakeDamage(float DamageAmount, struct FDamageEvent const&
	DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (MyHP <= 0.0f)
	{
		return 0.0f;
	}

	if (FinalDamage > 0.f)
	{
		MyHP -= FinalDamage;
	}

	if (MyHP <= 0)
	{
		if (Name == "ThinLong")
		{
			TVAnim->SetDeadAnim();
			SetActorEnableCollision(false);
			Controller->Destroy();
		}

		if (Name == "FatLong")
		{
			FVAnim->SetDeadAnim();
			SetActorEnableCollision(false);
			Controller->Destroy();
		}

		if (Name == "MidBoss")
		{
			MBAnim->SetDeadAnim();
			Controller->Destroy();
		}

		if (Name == "FinalBoss")
		{
			FBAnim->SetDeadAnim();
			SetActorEnableCollision(false);
			Controller->Destroy();
		}
		
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP : %f"), MyHP));
		//OnHit(FinalDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}
	//PlayAnimMontage(BeHit_AnimMontage, 1.0f);

	return FinalDamage;
}


UAnimMontage* AThinLongVillain::get_montage() const
{
	return montage;
}

void AThinLongVillain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어 뷰포인트 가져오기
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
}

void AThinLongVillain::BeginPlay()
{
	Super::BeginPlay();
}

float AThinLongVillain::get_HP() const
{
	return MyHP;
}

float AThinLongVillain::get_MaxHP() const
{
	return MyMaxHP;
}

void AThinLongVillain::set_HP(float const new_HP)
{
	MyHP = new_HP;
}