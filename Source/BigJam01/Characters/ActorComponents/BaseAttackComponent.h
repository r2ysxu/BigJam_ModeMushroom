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
enum class EStatusDebuffType : uint8 {
	VE_NONE		 UMETA(DisplayName = "None"),
	VE_POISON    UMETA(DisplayName = "Poison"),
	VE_TOXIN     UMETA(DispalyName = "Toxin"),
	VE_ZMD       UMETA(DisplayName = "ZMD")
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
	
	virtual void ApplyStatusToWeapon(EStatusDebuffType Debuff);
};
