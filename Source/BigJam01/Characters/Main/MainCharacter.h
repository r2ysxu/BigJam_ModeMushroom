// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../BaseCharacter.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../ActorComponents/ComboComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMainCharacter : public ABaseCharacter {
	GENERATED_BODY()

private:
	const float ROLL_VELOCITY = 500.f;
	const FName WeaponSocket = FName("weapon");

	FTimerHandle OnDodgeHandler;
	FTimerHandle OnDodgeStopHandler;
	FTimerHandle OnFlinchHandler;
	FTimerHandle OnStaminaHandler;

	void SetupHUDs();
	void InitiateAttack(EAttackType AttackType);

protected:
	volatile bool bAttacking = true;
	volatile bool bRollWindowOpen = true;
	volatile bool bRolling = false;
	volatile bool bFlinching = false;

	volatile bool bAttackLHeld = false;
	volatile bool bAttackRHeld = false;

	volatile bool bFocusing = false;

	uint32 EquipedWeaponIndex = 0;
	TArray<class AMeleeWeapon*> AvailableWeapons;
	class ABaseEnemy* FocusedTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float Stamina = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float StaminaRegenRate = .1f;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Weapons")
	TArray<FSpawnMeleeWeapon> EquippableWeaponClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class UComboComponent* ComboComponet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class UDirectionalAttackComponent* DaoComponet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class UChargeAttackComponent* ChargeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* FlinchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UComboHUD> ComboHudClass;
	class UComboHUD* ComboHud;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UPlayerStatHUD> PlayerHudClass;
	class UPlayerStatHUD* PlayerHud;
	
	bool CanMove();
	bool CanMoveAndAttack();

	void SetMovementDirection(FVector2D MovementVector);
	void OnDodgeRoll();
	void OnDodgeRollStop();
	void OnFlinchStop();
	
	virtual void BeginPlay() override;
	virtual void SetupWeapons() override;
	virtual void AttackL();
	virtual void AttackR();
	virtual void DodgeRoll();
	virtual void FocusEnemy();
	
	void EquipWeapon(uint32 WeaponIndex);

public:
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDodgeWindow(bool IsOpen);
	virtual bool OnHitTarget(class ABaseCharacter* Target);
	
	void SetIsAttacking(bool IsAttacking);
	class AMeleeWeapon* GetEquippedWeapon();
	void OnNextCombo();
	void OnComboReset();
	void OnStaminaRegen();
	FORCEINLINE bool GetIsAttacking() { return bAttacking; }
	FORCEINLINE virtual uint8 GetTeam() override { return 1; }
	FORCEINLINE bool GetIsDodging() { return bRolling; }
	FORCEINLINE bool GetIsCharging() { return bAttackRHeld; }
	FORCEINLINE UFUNCTION(BlueprintCallable) float GetStamina();
	bool HasCharged();
	UFUNCTION(BlueprintCallable) bool DrainStamina(float Value);
	UFUNCTION(BlueprintCallable) void OnHitByOpponent();
	UFUNCTION(BlueprintCallable) class UComboComponent* GetComboComponent();
	class UComboHUD* GetComboHud();

};
