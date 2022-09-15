// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatsKingdomCharacter.h"
#include "CatsKingdom.h"
#include "YongsaAnimInstance.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "CKAIController.h"
#include "CKWeapon.h"
#include "DrawDebugHelpers.h"
#include "CKProjectile.h"
#include "GameFramework/Actor.h"
//#include "CKLineTrace.h"
#include "YongsaAnimInstance.h"
//#include "CKCharacterWidget.h"
#include "Blueprint/UserWidget.h"
#include "Containers/UnrealString.h"

//////////////////////////////////////////////////////////////////////////
// ACatsKingdomCharacter

ACatsKingdomCharacter::ACatsKingdomCharacter()
	:MyHP(MyMaxHP)
	//HPBarWidget(CreateDefaultSubobject<UWidgetComponent>(TEXT("HPValue")))
{

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

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CKCharacter"));

	MyHP = 0.f;
	MyMaxHP = 1000.0f;
	// MyHP = MyMaxHP;

	IsAttacking = false;

	//isDuringAttack = false;

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	//GetMesh()->SetSimulatePhysics(true);

	MaxCombo = 4;
	AttackEndComboState();

	PrimaryActorTick.bCanEverTick = true;
	//CharacterStat = CreateDefaultSubobject<UCKCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->FRotator(0.0f, 0.0f, 50.0f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	AIControllerClass = ACKAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	FName Socket(TEXT("attach_socket"));
	if (GetMesh()->DoesSocketExist(Socket))
	{
		Heart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HEART"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON2(TEXT("/Game/Item_Data/Heart.Heart"));
		if (SK_WEAPON2.Succeeded())
		{
			Heart->SetStaticMesh(SK_WEAPON2.Object);
		}

		Heart->SetupAttachment(GetMesh(), Socket);
	}

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ACatsKingdomCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CKAnim = Cast<UYongsaAnimInstance>(GetMesh()->GetAnimInstance());
	//CKCHECK(nullptr != AnimInstance);

	CKAnim->OnMontageEnded.AddDynamic(this, &ACatsKingdomCharacter::OnAttackMontageEnded);

	CKAnim->OnAttackHitCheck.AddUObject(this, &ACatsKingdomCharacter::AttackCheck);

	CKAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		//CKLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			CKAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACatsKingdomCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACatsKingdomCharacter::SprintStart);
	//PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACatsKingdomCharacter::SprintStop);

	//PlayerInputComponent->BindAction("Dive", IE_Pressed, this, &ACatsKingdomCharacter::Dive);

	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACatsKingdomCharacter::Attack);
	
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACatsKingdomCharacter::Fire);

	//PlayerInputComponent->BindAction("Razer", IE_Pressed, this, &ACKLineTrace::Tick(float DeltaTime));

	//PlayerInputComponent->BindAxis("MoveForward", this, &ACatsKingdomCharacter::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &ACatsKingdomCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACatsKingdomCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACatsKingdomCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACatsKingdomCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACatsKingdomCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACatsKingdomCharacter::OnResetVR);
}

void ACatsKingdomCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//SetControlMode(EControlMode::NPC);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ACatsKingdomCharacter::OnResetVR()
{
	// If CatsKingdom is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in CatsKingdom.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACatsKingdomCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACatsKingdomCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

// 뛰기 시작 구현
void ACatsKingdomCharacter::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

// 뛰기 시작 구현
void ACatsKingdomCharacter::EquipSprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = EquipSprintSpeed;
}

// 뛰기 멈춤 구현
void ACatsKingdomCharacter::SprintStop()
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

// 뛰기 멈춤 구현
void ACatsKingdomCharacter::EquipSprintStop()
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

// 구르기 구현
void ACatsKingdomCharacter::Dive()
{
		if ((GetCharacterMovement()->MaxWalkSpeed == 500.f || GetCharacterMovement()->MaxWalkSpeed == 600.f) && !GetCharacterMovement()->IsFalling())
		{
			const FVector ForwardDir = this->GetActorRotation().Vector();
			LaunchCharacter(ForwardDir * DashDistance, true, true);

			CKAnim->PlayDiveMontage();

			Stamina = Stamina - 0.05;
		}
}

// 공격 애니메이션 재생
void ACatsKingdomCharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		CKAnim->PlayAttackMontage();
		CKAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ACatsKingdomCharacter::Fire()
{
	if (GetCharacterMovement()->MaxWalkSpeed == 500.f || GetCharacterMovement()->MaxWalkSpeed == 600.f)
	{
		CKAnim->PlayRunFireMontage();
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 200.f)
	{
		CKAnim->PlayWalkFireMontage();
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 0.f)
	{
		CKAnim->PlayFireMontage();
	}

	// 프로젝타일 발사
	if (ProjectileClass)
	{
		// 카메라 트랜스폼
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// MuzzleOffset 을 카메라 스페이스에서 월드 스페이스로 변환
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 조준을 약간 윗쪽으로
		MuzzleRotation.Pitch += 5.0f;
		//MuzzleRotation.Yaw += 4.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// 총구 위치에 발사체 스폰
			ACKProjectile* Projectile = World->SpawnActor<ACKProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 발사 방향
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}

		if (Ammo > 0)
			Ammo = Ammo - 1;

	}
}

void ACatsKingdomCharacter::Reload()
{
	if (ExtraAmmo >= 40 && Ammo < 40)
	{
		ExtraAmmo = ExtraAmmo - 40;
		Ammo = 40;
	}
}

void ACatsKingdomCharacter::Shoot()
{
	if (GetCharacterMovement()->MaxWalkSpeed == 500.f || GetCharacterMovement()->MaxWalkSpeed == 600.f)
	{
		CKAnim->PlayRunFireMontage();
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 200.f)
	{
		CKAnim->PlayWalkFireMontage();
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 0.f)
	{
		CKAnim->PlayFireMontage();
	}

	// 프로젝타일 발사
	if (ProjectileClass2)
	{
		// 카메라 트랜스폼
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// MuzzleOffset 을 카메라 스페이스에서 월드 스페이스로 변환
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 조준을 약간 윗쪽으로
		MuzzleRotation.Pitch += 5.0f;
		//MuzzleRotation.Yaw += 4.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// 총구 위치에 발사체 스폰
			ACKProjectile* Projectile = World->SpawnActor<ACKProjectile>(ProjectileClass2, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 발사 방향
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void ACatsKingdomCharacter::Stun()
{
	if (GetCharacterMovement()->MaxWalkSpeed == 500.f || GetCharacterMovement()->MaxWalkSpeed == 600.f)
	{
		CKAnim->PlayRunFireMontage();
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 200.f)
	{
		CKAnim->PlayWalkFireMontage();
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 0.f)
	{
		CKAnim->PlayFireMontage();
	}

	// 프로젝타일 발사
	if (ProjectileClass3)
	{
		// 카메라 트랜스폼
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// MuzzleOffset 을 카메라 스페이스에서 월드 스페이스로 변환
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 조준을 약간 윗쪽으로
		MuzzleRotation.Pitch += 5.0f;
		//MuzzleRotation.Yaw += 4.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// 총구 위치에 발사체 스폰
			ACKProjectile* Projectile = World->SpawnActor<ACKProjectile>(ProjectileClass3, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 발사 방향
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

// 데미지 처리
float ACatsKingdomCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const&
	DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount,	DamageEvent, EventInstigator, DamageCauser);

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
		
		CKAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP : %f"), MyHP));
		//OnHit(FinalDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}
	//PlayAnimMontage(BeHit_AnimMontage, 1.0f);

	return FinalDamage;
}

// 공격 범위
void ACatsKingdomCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(50.0f), Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5 + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	//FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//float DebugLifeTime = 1.0f;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}

}

/*void ACatsKingdomCharacter::FireCheck()
{
	FHitResult OutHit;
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = (Start + (ForwardVector * 10000.f));
	CollisionParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
}*/

/*void ACatsKingdomCharacter::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInsigator, class AActor* DamageCauser)
{
	PlayAnimMontage(BeHit_AnimMontage);
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInsigator, DamageCauser);
	}
}*/

void ACatsKingdomCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	IsAttacking = false;
	AttackEndComboState();
}

// 콤보 공격 시작
void ACatsKingdomCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// 콤보 공격 종료
void ACatsKingdomCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ACatsKingdomCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACatsKingdomCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACatsKingdomCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACatsKingdomCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACatsKingdomCharacter::BeginPlay()
{
	Super::BeginPlay();

	//PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	/*FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<ACKWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}*/
}

// Called every frame
void ACatsKingdomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*auto const uw = Cast<UCKCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (uw)
	{
		float tempNum = MyHP / MyMaxHP;
		uw->UpdateHPWidget(tempNum);
	}*/

	// 플레이어 뷰포인트 가져오기
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// LOG
	/*UE_LOG(LogTemp, Warning, TEXT("Location : %s, Rotation : %s"),
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString()
	)*/
}

float ACatsKingdomCharacter::get_HP() const
{
	return MyHP;
}

float ACatsKingdomCharacter::get_MaxHP() const
{
	return MyMaxHP;
}

void ACatsKingdomCharacter::set_HP(float const new_HP)
{
	MyHP = new_HP;
}