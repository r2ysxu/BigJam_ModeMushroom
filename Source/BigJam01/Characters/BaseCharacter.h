// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class BIGJAM01_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

protected:
	bool bAttacking = false;

	virtual void BeginPlay() override;
	virtual void SetupWeapons() {}

public:
	ABaseCharacter();

	virtual bool OnHitTarget(class ABaseCharacter* Target);
	virtual void OnHitByOpponent() {}
	FORCEINLINE virtual uint8 GetTeam() { return 0; }
	FORCEINLINE virtual bool GetIsAttacking() { return bAttacking; }
};
