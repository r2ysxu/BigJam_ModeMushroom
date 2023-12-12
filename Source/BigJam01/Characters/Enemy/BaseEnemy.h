// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS(Abstract)
class BIGJAM01_API ABaseEnemy : public ABaseCharacter {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDebuffComponent* DebuffComponent;

	bool bSleeping = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ABaseEnemy();

	virtual bool CheckAlive() override;
	virtual void TakeHitDamage(float Damage);
	virtual float OnHitByOpponent(float Damage, enum EStatusDebuffType Status) override;
	FORCEINLINE virtual uint8 GetTeam() { return 2; }
	FORCEINLINE bool GetIsSleeping() { return bSleeping; }
	void SetIsSleeping(bool IsSleeping);
};
