// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../BaseCharacter.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../ActorComponents/BaseAttackComponent.h"

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
class BIGJAM01_API AMainCharacter : public ABaseCharacter {
	GENERATED_BODY()

private:
	const float ROLL_VELOCITY = 500.f;
	const FName WeaponSocket = FName("weapon");

	FTimerHandle OnDodgeHandler;
	FTimerHandle OnDodgeStopHandler;
	FTimerHandle OnFlinchHandler;
	FTimerHandle OnStaminaHandler;
	FTimerHandle OnMusicHandler;
	FTimerHandle OnHealthRegenHandler;
	FTimerHandle OnDeathHandler;

	void SetupHUDs();
	void InitiateAttack(EAttackType AttackType);

protected:
	volatile bool bAttacking = false;
	volatile bool bRollWindowOpen = true;
	volatile bool bRolling = false;
	volatile bool bFlinching = false;

	volatile bool bAttackLHeld = false;
	volatile bool bAttackRHeld = false;

	volatile bool bFocusing = false;

	uint32 EquipedWeaponIndex = 0;
	TArray<class AMeleeWeapon*> AvailableWeapons;
	class ABaseEnemy* FocusedTarget = nullptr;
	class ABaseCharacter* LastHitEnemy;

	class UEnemyReactionComponent* EnemyReactionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float Stamina = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float StaminaRegenRate = .1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DodgeStaminaDrain = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegen = .05f;

	float MasterVolume = 1.f;
	float BGMVolume = 1.f;
	float SFXVolume = 1.f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* BGMBattleSound;
	class UAudioComponent* BGMBattleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* BGMSereneSound;
	class UAudioComponent* BGMSereneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UPlayerStatHUD> PlayerHudClass;
	class UPlayerStatHUD* PlayerHud;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	
	bool CanMove();
	bool CanMoveAndAttack();

	void SetMovementDirection(FVector MovementVector);
	void OnDodgeRoll();
	void OnDodgeRollStop();
	void OnFlinchStop();
	void OnCheckBGMusic();
	
	virtual void BeginPlay() override;
	virtual void SetupWeapons() override;
	virtual void AttackL();
	virtual void AttackR();
	virtual void AttackQ();
	virtual void AttackE();
	virtual void DodgeRoll();
	
	void EquipWeapon(uint32 WeaponIndex);

public:
	AMainCharacter();

	void SetDodgeWindow(bool IsOpen);
	virtual bool OnHitTarget(class ABaseCharacter* Target, float Damage, enum EStatusDebuffType Status) override;
	virtual float OnHitByOpponent(float Damage, enum EStatusDebuffType Status) override;
	virtual bool CheckAlive();
	
	void SetIsAttacking(bool IsAttacking);
	class AMeleeWeapon* GetEquippedWeapon();
	void OnNextCombo();
	void OnComboReset();
	void OnStaminaRegen();
	void OnHealthRegen();
	bool HasCharged();
	void DeathRestart();
	FORCEINLINE bool GetIsAttacking() { return bAttacking; }
	FORCEINLINE virtual uint8 GetTeam() override { return 1; }
	FORCEINLINE bool GetIsDodging() { return bRolling; }
	FORCEINLINE bool GetIsCharging() { return bAttackRHeld; }
	FORCEINLINE UFUNCTION(BlueprintCallable) float GetStamina();
	UFUNCTION(BlueprintCallable) void OnPlayerStart();
	UFUNCTION(BlueprintCallable) bool DrainStamina(float Value);
	UFUNCTION(BlueprintCallable) class UComboComponent* GetComboComponent();
	class UEnemyReactionComponent* GetEnemyReactionComponent();

	FORCEINLINE float GetMasterVolume() { return MasterVolume; }
	FORCEINLINE float GetBGMVolume() { return BGMVolume; }
	FORCEINLINE float GetSFXVolume() { return SFXVolume; }
	void SetVolumes(float Master, float Music, float Sfx);

};
