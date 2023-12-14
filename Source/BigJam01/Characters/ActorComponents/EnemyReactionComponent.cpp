// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyReactionComponent.h"
#include "../../Characters/Enemy/Melee/MeleeEnemy.h"

// Sets default values for this component's properties
UEnemyReactionComponent::UEnemyReactionComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UEnemyReactionComponent::BeginPlay() {
	Super::BeginPlay();
}

void UEnemyReactionComponent::Subscribe(AMeleeEnemy* Enemy) {
	SubscribedEnemies.Add(Enemy);
}

void UEnemyReactionComponent::NotifyDodge() {
	for (AMeleeEnemy* enemy : SubscribedEnemies) {
		enemy->DashBack();
	}
}

void UEnemyReactionComponent::Unsubscribe(AMeleeEnemy* Enemy) {
	SubscribedEnemies.Remove(Enemy);
}
