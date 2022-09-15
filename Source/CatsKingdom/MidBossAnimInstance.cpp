// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBossAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// ��������2 �Ǵ���� �ִϸ��̼�
UMidBossAnimInstance::UMidBossAnimInstance()
{
	Speed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Villain_Data/MidBoss_Data/Standing_Walk_Forward__3__Montage.Standing_Walk_Forward__3__Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UMidBossAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}