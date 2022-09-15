// Fill out your copyright notice in the Description page of Project Settings.

#include "CKProjectile.h"
#include "CatsKingdom.h"
#include "CatsKingdomCharacter.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Pawn.h"
//#include "/Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"


// Sets default values
ACKProjectile::ACKProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    // ��ü�� �ܼ� �ݸ��� ǥ������ ���
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACKProjectile::OnOverlapBegin);

    // ��ü�� �ݸ��� �ݰ��� ����
    CollisionComponent->InitSphereRadius(3.0f);
    //CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);


    // ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� ����
    RootComponent = CollisionComponent;

    // ������Ÿ���� �����Ʈ�� ����
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.0f;
}

// Called when the game starts or when spawned
void ACKProjectile::BeginPlay()
{
    Super::BeginPlay();

    // 2�� �� ����
    SetLifeSpan(2.0f);
}

// Called every frame
void ACKProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}



void ACKProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ACKProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->IsA(ACatsKingdomCharacter::StaticClass()))
        {
            Destroy();
        }
    }
}

// ������Ÿ�Ͽ� ���� ������ ȣ��
void ACKProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
    AController* GetController(), FVector NormalImpulse, const FHitResult& Hit)
{
    FHitResult HitResult;
    FDamageEvent DamageEvent;

    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        HitResult.Actor->TakeDamage(20.0f, DamageEvent, GetController(), this);
        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 20.0f, Hit.ImpactPoint);
        Destroy();
    }

    /*if (HitResult.Actor.IsValid())
    {
        FDamageEvent DamageEvent;
        HitResult.Actor->TakeDamage(20.0f, DamageEvent, GetController(), this);
    }*/
}