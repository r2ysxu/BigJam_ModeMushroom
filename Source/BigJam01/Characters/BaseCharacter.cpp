// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "ActorComponents/BaseAttackComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter() {
	PrimaryActorTick.bCanEverTick = false;
}

bool ABaseCharacter::CheckAlive() {
	return Health >= 0;
}

bool ABaseCharacter::OnHitTarget(ABaseCharacter* Target, float Damage, EComboDebuffType Status) { return false; }

EComboDebuffType ABaseCharacter::GetCurrentDebuff() {
	return DebuffStatus;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();	
}

