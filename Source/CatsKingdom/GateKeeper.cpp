// Fill out your copyright notice in the Description page of Project Settings.


#include "GateKeeper.h"
#include "KeeperAnimInstance.h"
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
#include "GKSplash.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"

AGateKeeper::AGateKeeper()
{
	MyHP = 0.f;
	MyMaxHP = 100.0f;
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

	// ���� ����

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	/*FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Splash = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SPLASH"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> GK_SPLASH(TEXT("/Game/Villain_Data/Keeper_Data/Splash2.Splash2"));
		if (GK_SPLASH.Succeeded())
		{
			Splash->SetSkeletalMesh(GK_SPLASH.Object);
		}

		Splash->SetupAttachment(GetMesh(), WeaponSocket);
	}*/

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CKCharacter"));

	/*GetMesh()->SetSimulatePhysics(true);

	if (Controller)
	{
		Controller->Destroy();
	}*/
}

void AGateKeeper::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GKAnim = Cast<UKeeperAnimInstance>(GetMesh()->GetAnimInstance());
	//CKCHECK(nullptr != AnimInstance);

	GKAnim->OnMontageEnded.AddDynamic(this, &AGateKeeper::OnAttackMontageEnded);

	GKAnim->OnAttackHitCheck.AddUObject(this, &AGateKeeper::AttackCheck);
}

void AGateKeeper::Attack()
{
	
	AttackCheck();
	IsAttacking = true;
	
	return;
}

float AGateKeeper::TakeDamage(float Damage, struct FDamageEvent const&
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
		GKAnim->SetDeadAnim();
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

/*void AGateKeeper::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	//PlayAnimMontage(BeHit_AnimMontage);
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}*/

void AGateKeeper::AttackCheck()
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
			GKAnim->PlayAttackMontage();
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(10.0f, DamageEvent, GetController(), this);
		}
	}
}

void AGateKeeper::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void AGateKeeper::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AGateKeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �÷��̾� ������Ʈ ��������
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
}

void AGateKeeper::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AGKSplash>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
}

float AGateKeeper::get_HP() const
{
	return MyHP;
}

float AGateKeeper::get_MaxHP() const
{
	return MyMaxHP;
}

void AGateKeeper::set_HP(float const new_HP)
{
	MyHP = new_HP;
}
