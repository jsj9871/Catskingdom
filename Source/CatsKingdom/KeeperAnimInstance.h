// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KeeperAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class CATSKINGDOM_API UKeeperAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UKeeperAnimInstance();
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	void SetDeadAnim() { IsDead = true; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	// ���� ��Ÿ�� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
};
