// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter() {
	PrimaryActorTick.bCanEverTick = false;
}

bool ABaseCharacter::OnHitTarget(ABaseCharacter* Target) { return false; }

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();
	
}

