// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorComponents/BaseAttackComponent.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class BIGJAM01_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

protected:
	bool bAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EStatusDebuffType DebuffStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Health = 1.f;

	virtual void BeginPlay() override;
	virtual void SetupWeapons() {}

public:
	ABaseCharacter();

	virtual bool CheckAlive();
	virtual bool OnHitTarget(class ABaseCharacter* Target, float Damage, EStatusDebuffType Status);
	virtual float OnHitByOpponent(float Damage, EStatusDebuffType Status) { return 0.f; }
	FORCEINLINE virtual uint8 GetTeam() { return 0; }
	FORCEINLINE virtual bool GetIsAttacking() { return bAttacking; }
};
