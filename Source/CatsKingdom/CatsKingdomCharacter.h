	// Copyright Epic Games, Inc. All Rights Reserved.

	#pragma once

	#include "CoreMinimal.h"
	#include "GameFramework/Character.h"
	//#include "GameFramework/Actor.h"
	#include "PhysicsEngine/PhysicsHandleComponent.h"
	#include "CatsKingdomCharacter.generated.h"


	UCLASS(config = Game)
	class ACatsKingdomCharacter : public ACharacter
	{
		GENERATED_BODY()

			/** Camera boom positioning the camera behind the character */
			UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
			class USpringArmComponent* CameraBoom;

		/** Follow camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
			class UCameraComponent* FollowCamera;

	public:
		ACatsKingdomCharacter();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
			float MyHP;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
			float MyMaxHP;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
			float Stamina;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
			int Ammo = 40;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
			int ExtraAmmo = 80;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyState)
			int AmmoCharge = 40;

		UFUNCTION(BlueprintCallable)
			void Reload();

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			float AttackRange;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			float AttackRadius;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			bool IsAttacking;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
			float SprintSpeed = 600;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
			float EquipSprintSpeed = 500;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
			float CoolTime = 1.f;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
			float DiveCoolTime = 1.f;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
			float SkillCoolTime = 1.f;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
			float StunCoolTime = 1.f;

		UPROPERTY(VisibleAnywhere, Category = Weapon)
			USkeletalMeshComponent* Weapon;

		UPROPERTY(VisibleAnywhere, Category = Heart)
			UStaticMeshComponent* Heart;

		// 카메라 위치에서의 총구 오프셋
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			FVector MuzzleOffset;

		// 스폰시킬 프로젝타일 클래스
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			TSubclassOf<class ACKProjectile> ProjectileClass;

		// 스폰시킬 프로젝타일 클래스
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			TSubclassOf<class ACKProjectile> ProjectileClass2;

		// 스폰시킬 프로젝타일 클래스
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			TSubclassOf<class ACKProjectile> ProjectileClass3;

	public:
		virtual void Tick(float DeltaTime) override;
		//virtual void PostInitializeComponents() override;
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const&
			DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
		virtual void PossessedBy(AController* NewController) override;

		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			float BaseTurnRate;

		/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			float BaseLookUpRate;

		UPROPERTY(EditDefaultsOnly, Category = Pawn)
			UAnimMontage* BeHit_AnimMontage;

		UPROPERTY(EditDefaultsOnly, Category = Pawn)
			UAnimMontage* Attack_AnimMontage;

		UPROPERTY(EditDefaultsOnly, Category = "MyFX")
			UParticleSystem* HitFX;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Body)
			UPhysicsHandleComponent* PhysicsHandle;

		//void Attack_Melee();
		//void Attack_Melee_End();
		//void ShowFX();

		//UPROPERTY(BlueprintReadWrite)
			//class UWidgetComponent* HPBarWidget;

		float get_HP()const;
		float get_MaxHP()const;
		void set_HP(float const new_HP);

		UPROPERTY(EditDefaultsOnly, Category = MyState)
			FName Name;

	private:
		virtual void PostInitializeComponents() override;

		UFUNCTION(BlueprintCallable)
			void Fire();

		UFUNCTION(BlueprintCallable)
			void Shoot();

		UFUNCTION(BlueprintCallable)
			void Stun();

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			bool CanNextCombo;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			bool IsComboInputOn;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			int32 CurrentCombo;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			int32 MaxCombo;

		UPROPERTY()
			class UYongsaAnimInstance* CKAnim;

		UFUNCTION(BlueprintCallable)
		void Attack();

		UFUNCTION(BlueprintCallable)
		void AttackCheck();

		float Speed = 200.f;	// 기본 속도

		// 뛰기
		UFUNCTION(BlueprintCallable)
		void SprintStart();	// 뛰기 시작
		UFUNCTION(BlueprintCallable)
			void EquipSprintStart();	// 뛰기 시작
		UFUNCTION(BlueprintCallable)
		void SprintStop();	// 뛰기 멈춤
		UFUNCTION(BlueprintCallable)
			void EquipSprintStop();	// 뛰기 멈춤

		// float SprintSpeed = 500.f;	// 뛰는 속도

		// 대쉬
		UFUNCTION(BlueprintCallable)
		void Dive();

		float DashDistance = 2000;	// 대쉬 거리

		float Reach = 100.f;

	protected:
		virtual void BeginPlay() override;

		UFUNCTION()
			void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

		//virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInsigator, class AActor* DamageCauser);

		void AttackStartComboState();
		void AttackEndComboState();

		/** Resets HMD orientation in VR. */
		void OnResetVR();

		UFUNCTION(BlueprintCallable)
		/** Called for forwards/backward input */
		void MoveForward(float Value);

		UFUNCTION(BlueprintCallable)
		/** Called for side to side input */
		void MoveRight(float Value);

		/**
		 * Called via input to turn at a given rate.
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void TurnAtRate(float Rate);

		/**
		 * Called via input to turn look up/down at a given rate.
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void LookUpAtRate(float Rate);

		/** Handler for when a touch input begins. */
		void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

		/** Handler for when a touch input stops. */
		void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	protected:
		// APawn interface
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		// End of APawn interface

	public:
		/** Returns CameraBoom subobject **/
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
		/** Returns FollowCamera subobject **/
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	};

