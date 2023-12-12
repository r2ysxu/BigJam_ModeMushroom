// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "ActorComponents/BaseAttackComponent.h"
#include "ActorComponents/DebuffComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter() {
	PrimaryActorTick.bCanEverTick = false;
}

bool ABaseCharacter::CheckAlive() {
	return Health >= 0;
}

bool ABaseCharacter::OnHitTarget(ABaseCharacter* Target, float Damage, EStatusDebuffType Status) { return false; }

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();	
}

