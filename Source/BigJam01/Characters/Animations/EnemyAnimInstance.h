// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class BIGJAM01_API UEnemyAnimInstance : public UBasicAnimInstance {
	GENERATED_BODY()

private:
	class ABaseEnemy* Owner;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual class ABaseEnemy* GetEnemyOwner();

	UFUNCTION(BlueprintCallable) void AnimLockAcquire();
	UFUNCTION(BlueprintCallable) void AnimLockRelease();
};

