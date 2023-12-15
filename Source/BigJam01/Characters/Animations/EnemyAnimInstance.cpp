// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAnimInstance.h"
#include "../Enemy/BaseEnemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	Owner = Cast<ABaseEnemy>(TryGetPawnOwner());
}

ABaseEnemy* UEnemyAnimInstance::GetEnemyOwner() {
	return Owner;
}

void UEnemyAnimInstance::AnimLockRelease() {
	Owner->SetInAnimLock(false);
}

void UEnemyAnimInstance::AnimLockAcquire() {
	Owner->SetInAnimLock(true);
}