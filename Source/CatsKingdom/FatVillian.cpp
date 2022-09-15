// Fill out your copyright notice in the Description page of Project Settings.


#include "FatVillian.h"
#include "FatAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "CKAIController.h"
//#include "CKCharacterWidget.h"
#include "CatsKingdomCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

AFatVillian::AFatVillian()
{
	MyHP = 0.f;
	MyMaxHP = 200.0f;
	MyHP = MyMaxHP;

	IsAttacking = false;

	//TVMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TVMesh"));
	//RootComponent = TVMesh;

	/*HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPValue"));
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}*/

	// 공격 범위

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CKCharacter"));

	/*GetMesh()->SetSimulatePhysics(true);

	if (Controller)
	{
		Controller->Destroy();
	}*/
}

void AFatVillian::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FVAnim = Cast<UFatAnimInstance>(GetMesh()->GetAnimInstance());
	//CKCHECK(nullptr != AnimInstance);

	FVAnim->OnMontageEnded.AddDynamic(this, &AFatVillian::OnAttackMontageEnded);

	FVAnim->OnAttackHitCheck.AddUObject(this, &AFatVillian::AttackCheck);
}

void AFatVillian::Attack()
{
	AttackCheck();
	IsAttacking = true;

	return;
}

float AFatVillian::TakeDamage(float Damage, struct FDamageEvent const&
	DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

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
		//TVMesh->getMesh()->SetMaterial();
		FVAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		Controller->Destroy();
	}
	else
	{
		//OnHit(FinalDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP : %f"), MyHP));
	}


	return FinalDamage;
}

/*void AThinVillain::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	//PlayAnimMontage(BeHit_AnimMontage);
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}*/

void AFatVillian::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(50.0f), Params);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FVAnim->PlayAttackMontage();
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(80.0f, DamageEvent, GetController(), this);
		}
	}
}

void AFatVillian::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void AFatVillian::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AFatVillian::Tick(float DeltaTime)
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

void AFatVillian::BeginPlay()
{
	Super::BeginPlay();
}

float AFatVillian::get_HP() const
{
	return MyHP;
}

float AFatVillian::get_MaxHP() const
{
	return MyMaxHP;
}

void AFatVillian::set_HP(float const new_HP)
{
	MyHP = new_HP;
}
