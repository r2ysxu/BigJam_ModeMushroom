// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "../ActorComponents/BaseAttackComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay() {
	Super::BeginPlay();
}

float ABaseEnemy::CalculateDamage(float BaseDamage, EComboDebuffType Status) {
	switch (Status) {
	case EComboDebuffType::VE_NONE:
	case EComboDebuffType::VE_POISON:
	case EComboDebuffType::VE_SUNDER:
	case EComboDebuffType::VE_SLEEP:
		return BaseDamage;
	case EComboDebuffType::VE_BURST:
		return BaseDamage * 1.2; // 20%
	case EComboDebuffType::VE_HEAVY:
		return BaseDamage * 1.1; // 10%
	case EComboDebuffType::VE_CHARGE:
		return BaseDamage * 1.2;
	default: return BaseDamage;
	}
}

void ABaseEnemy::ApplyDebuff(EComboDebuffType Status) {
	switch (Status) {
	case EComboDebuffType::VE_NONE:
	case EComboDebuffType::VE_HEAVY:
	case EComboDebuffType::VE_SLEEP:
	case EComboDebuffType::VE_BURST:
	case EComboDebuffType::VE_CHARGE:
		DebuffStatus = EComboDebuffType::VE_NONE;
		break;
	case EComboDebuffType::VE_POISON:
	case EComboDebuffType::VE_SUNDER:
		DebuffStatus = Status;
	}
}

void ABaseEnemy::OnHitByOpponent(float Damage, EComboDebuffType Status) {
	float totalDamage = CalculateDamage(Damage, Status);
	if (Status == EComboDebuffType::VE_HEAVY || Status == EComboDebuffType::VE_CHARGE) {
		if (DebuffStatus == EComboDebuffType::VE_POISON) {
			totalDamage *= 1.2;
		} else if (DebuffStatus == EComboDebuffType::VE_SUNDER) {
			totalDamage *= 1.3;
		}
	}
	Health -= FMath::Min(Health, totalDamage);
	ApplyDebuff(Status);
}

bool ABaseEnemy::CheckAlive() {
	return Health > 0;
}