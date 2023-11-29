// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS(Abstract)
class BIGJAM01_API ABaseEnemy : public ACharacter {
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ABaseEnemy();

	virtual void OnHitByOpponent() {}
};
