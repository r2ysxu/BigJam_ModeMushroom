// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class BIGJAM01_API ABaseWeapon : public AActor {
	GENERATED_BODY()

protected:
	class ABaseCharacter* Owner;

	virtual void BeginPlay() override;

public:
	ABaseWeapon();

	virtual void SetWielder(class ABaseCharacter* Wielder);
};
