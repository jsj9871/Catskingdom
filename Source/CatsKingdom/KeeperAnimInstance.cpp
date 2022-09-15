// Fill out your copyright notice in the Description page of Project Settings.


#include "KeeperAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// 스테이지2 악당냥이 애니메이션
UKeeperAnimInstance::UKeeperAnimInstance()
{
	Speed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Villain_Data/Keeper_Data/Standing_Melee_Attack_Downward_Montage.Standing_Melee_Attack_Downward_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UKeeperAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}