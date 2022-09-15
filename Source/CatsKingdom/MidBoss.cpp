// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss.h"

// Sets default values
AMidBoss::AMidBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMidBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMidBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMidBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

