// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "../ActorComponents/BaseAttackComponent.h"
#include "../ActorComponents/DebuffComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DebuffComponent = CreateDefaultSubobject<UDebuffComponent>(TEXT("DebuffComponent"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay() {
	Super::BeginPlay();
}

bool ABaseEnemy::CheckAlive() {
	return Health > 0;
}

void ABaseEnemy::TakeHitDamage(float Damage) {
	bSleeping = false;
	Health -= FMath::Min(Health, Damage);
}

float ABaseEnemy::OnHitByOpponent(float Damage, EStatusDebuffType Status) {
	TakeHitDamage(Damage);
	return DebuffComponent->ApplyStatus(Status);
}

void ABaseEnemy::SetIsSleeping(bool IsSleeping) {
	bSleeping = IsSleeping;
}
