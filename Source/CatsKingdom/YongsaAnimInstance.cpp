// Fill out your copyright notice in the Description page of Project Settings.


#include "YongsaAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// 주인공 용사 애니메이션
UYongsaAnimInstance::UYongsaAnimInstance()
{
	Speed = 0.0f;
	IsInAir = false;
	IsDead = false;

	// 공격 몽타주 파일 찾아 적용
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/ThirdPersonCPP/Blueprints/Walking__6__Skeleton_Montage.Walking__6__Skeleton_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// 뛰는 격발 몽타주 파일 찾아 적용
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RUNFIRE_MONTAGE(TEXT("/Game/ThirdPersonCPP/Blueprints/Pistol_Run_Montage.Pistol_Run_Montage"));

	if (RUNFIRE_MONTAGE.Succeeded())
	{
		RunFireMontage = RUNFIRE_MONTAGE.Object;
	}

	// 걷는 격발 몽타주 파일 찾아 적용
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WALKFIRE_MONTAGE(TEXT("/Game/ThirdPersonCPP/Blueprints/Pistol_Walk_Montage.Pistol_Walk_Montage"));

	if (WALKFIRE_MONTAGE.Succeeded())
	{
		WalkFireMontage = WALKFIRE_MONTAGE.Object;
	}

	// 격발 몽타주 파일 찾아 적용
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FIRE_MONTAGE(TEXT("/Game/ThirdPersonCPP/Blueprints/Pistol_Idle_Montage.Pistol_Idle_Montage"));

	if (FIRE_MONTAGE.Succeeded())
	{
		FireMontage = FIRE_MONTAGE.Object;
	}

	// 대쉬	몽타주 파일 찾아 적용
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DIVE_MONTAGE(TEXT("/Game/ThirdPersonCPP/Blueprints/DiveMontage.DiveMontage"));

	if (DIVE_MONTAGE.Succeeded())
	{
		DiveMontage = DIVE_MONTAGE.Object;
	}
}

// 애니메이션 적용 위한 캐릭터 위치 정의
void UYongsaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		Speed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

// 공격 몽타주 실행 함수 구현
void UYongsaAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UYongsaAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage, 1.0f);
}

void UYongsaAnimInstance::PlayWalkFireMontage()
{
	Montage_Play(WalkFireMontage, 1.0f);
}

void UYongsaAnimInstance::PlayRunFireMontage()
{
	Montage_Play(RunFireMontage, 1.0f);
}

void UYongsaAnimInstance::PlayDiveMontage()
{
	Montage_Play(DiveMontage, 1.0f);
}

void UYongsaAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UYongsaAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UYongsaAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UYongsaAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

