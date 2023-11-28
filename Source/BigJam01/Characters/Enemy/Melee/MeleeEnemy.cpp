// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"

// Sets default values
AMeleeEnemy::AMeleeEnemy() {
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMeleeEnemy::BeginPlay() {
	Super::BeginPlay();
	SpawnDefaultController();
}

