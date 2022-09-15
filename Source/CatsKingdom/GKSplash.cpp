// Fill out your copyright notice in the Description page of Project Settings.


#include "GKSplash.h"

// Sets default values
AGKSplash::AGKSplash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Splash = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SPLASH"));
    RootComponent = Splash;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SPLASH(TEXT("/Game/Villain_Data/Keeper_Data/Splash2.Splash2"));
    if (SK_SPLASH.Succeeded())
    {
        Splash->SetSkeletalMesh(SK_SPLASH.Object);
    }
    Splash->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AGKSplash::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGKSplash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

