// Fill out your copyright notice in the Description page of Project Settings.


#include "ThinAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// 스테이지2 악당냥이 애니메이션
UThinAnimInstance::UThinAnimInstance()
{
	Speed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	ATTACK_MONTAGE(TEXT("/Game/Villain_Data/Thin_Data/ThinAttackMontage.ThinAttackMontage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UThinAnimInstance::PlayAttackMontage()
{
		Montage_Play(AttackMontage, 1.0f);
}