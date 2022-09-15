// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatsKingdomCharacter.h"
//#include "GameFramework/Character.h"
//#include "CKCharacterWidget.h"
#include "GateKeeper.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

/**
 *
 */
UCLASS()
class CATSKINGDOM_API AGateKeeper : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
		virtual float TakeDamage(float Damage, struct FDamageEvent const&
			DamageEvent, class AController* EventInstigator, AActor* DamageCouser) override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
		void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	AGateKeeper();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
		float MyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
		float MyMaxHP;

	float get_HP()const;
	float get_MaxHP()const;
	void set_HP(float const new_HP);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Splash;

	//UPROPERTY(BlueprintReadWrite)
		//class UWidgetComponent* HPBarWidget;

	//UPROPERTY(EditDefaultsOnly, Category = Pawn)
		//UAnimMontage* BeHit_AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = MyState)
		FName Name;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
		class UBehaviorTree* Behavior;

protected:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//UFUNCTION(BlueprintCallable)
	//virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInsigator, class AActor* DamageCauser);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY()
		class UKeeperAnimInstance* GKAnim;

	//UPROPERTY(VisibleAnywhere)
		//class USkelatalMeshComponent* TVMesh;

	//UPROPERTY(EditAnywhere)
		//class UMaterial* OnMaterial;

private:
	void AttackCheck();
};

