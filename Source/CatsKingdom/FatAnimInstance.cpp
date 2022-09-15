// Fill out your copyright notice in the Description page of Project Settings.


#include "FatAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// 스테이지2 악당냥이 애니메이션
UFatAnimInstance::UFatAnimInstance()
{
	Speed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Villain_Data/Fat_Data/Right_Hook_Montage.Right_Hook_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UFatAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}