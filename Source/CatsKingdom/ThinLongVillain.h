// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatsKingdomCharacter.h"
#include "Animation/AnimMontage.h"
#include "CombatInterface.h"
#include "ThinLongVillain.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CATSKINGDOM_API AThinLongVillain : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	AThinLongVillain();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, struct FDamageEvent const&
		DamageEvent, class AController* EventInstigator, AActor* DamageCouser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
		float MyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
		float MyMaxHP;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACKProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = MyState)
		FName Name;

	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree* BotBehavior;

	UPROPERTY(EditAnywhere, Category = Behavior)
		UAnimMontage* Attack_Melee;

	UFUNCTION(BlueprintCallable)
	virtual int melee_attack_Implementation() override;

	UAnimMontage* get_montage() const;

	float get_HP()const;
	float get_MaxHP()const;
	void set_HP(float const new_HP);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* montage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY()
		class UThinAnimInstance* TVAnim;

	UPROPERTY()
		class UFatAnimInstance* FVAnim;

	UPROPERTY()
		class UMidBossAnimInstance* MBAnim;

	UPROPERTY()
		class UFinalBossAnimInstance* FBAnim;
};
