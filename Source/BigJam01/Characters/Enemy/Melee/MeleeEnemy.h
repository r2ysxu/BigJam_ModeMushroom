// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class BIGJAM01_API AMeleeEnemy : public ACharacter {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AMeleeEnemy();
};
