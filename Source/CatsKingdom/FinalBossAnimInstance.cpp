// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalBossAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// 스테이지2 악당냥이 애니메이션
UFinalBossAnimInstance::UFinalBossAnimInstance()
{
	Speed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Villain_Data/FinalBoss_Data/Standing_Walk_Forward__4__Montage.Standing_Walk_Forward__4__Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UFinalBossAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}