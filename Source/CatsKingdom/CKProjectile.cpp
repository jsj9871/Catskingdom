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


    // 구체를 단순 콜리전 표현으로 사용
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACKProjectile::OnOverlapBegin);

    // 구체의 콜리전 반경을 설정
    CollisionComponent->InitSphereRadius(3.0f);
    //CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);


    // 루트 컴포넌트를 콜리전 컴포넌트로 설정
    RootComponent = CollisionComponent;

    // 프로젝타일의 무브먼트를 구동
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

    // 2초 후 삭제
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

// 프로젝타일에 무언가 맞으면 호출
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