// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	VE_L UMETA(DisplayName = "Attack_L"),
	VE_R UMETA(DisplayName = "Attack_R")
};

UENUM(BlueprintType)
enum class EComboDebuffType : uint8 {
	VE_NONE		 UMETA(DisplayName = "None"),
	VE_HEAVY	 UMETA(DisplayName = "Heavy"),
	VE_POISON	 UMETA(DisplayName = "Poison"),
	VE_SUNDER	 UMETA(DisplayName = "Sunder"),
	VE_SLEEP	 UMETA(DisplayName = "Sleep"),
	VE_BURST	 UMETA(DisplayName = "Burst"),
	VE_CHARGE	 UMETA(DisplayName = "Charge"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UBaseAttackComponent : public UActorComponent {
	GENERATED_BODY()

protected:
	FTimerHandle OnAttackHandler;

	class AMainCharacter* Owner;
	class AMeleeWeapon* Weapon;

	virtual void BeginPlay() override;

public:
	UBaseAttackComponent();
	
	virtual void ApplyStatusToWeapon(EComboDebuffType Debuff);
};
