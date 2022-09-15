// Fill out your copyright notice in the Description page of Project Settings.


#include "CKWeapon.h"

// Sets default values
ACKWeapon::ACKWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
    RootComponent = Weapon;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/Weapon/Pistol.Pistol"));
    if (SK_WEAPON.Succeeded())
    {
        Weapon->SetSkeletalMesh(SK_WEAPON.Object);
    }
    Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ACKWeapon::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ACKWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

