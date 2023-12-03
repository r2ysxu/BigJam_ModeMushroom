// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	VE_L UMETA(DisplayName = "Attack_L"),
	VE_R UMETA(DisplayName = "Attack_R")
};

UENUM(BlueprintType)
enum class EComboDebuffType : uint8 {
	VE_NONE		 UMETA(DisplayName = "None"),
	VE_POISON	 UMETA(DisplayName = "Poison"),
	VE_SUNDER	 UMETA(DisplayName = "Sunder"),
	VE_SLEEP	 UMETA(DisplayName = "Sleep"),
	VE_BURST	 UMETA(DisplayName = "Burst"),
	VE_PROC		 UMETA(DisplayName = "Proc"),
};

USTRUCT(BlueprintType)
struct FAttackComboNode {
	GENERATED_BODY()

	FAttackComboNode() : Debuff(EComboDebuffType::VE_NONE) {}
	FAttackComboNode(EComboDebuffType StatusDebuff) : Debuff(StatusDebuff) {}

	EComboDebuffType Debuff;
	struct FAttackComboNode* L = nullptr;
	struct FAttackComboNode* R = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UComboComponent : public UActorComponent {
	GENERATED_BODY()

private:
	class AMainCharacter* Owner;
	class AMeleeWeapon* Weapon;
	FTimerHandle OnAttackHandler;

	volatile int ComboChain = 0;
	volatile bool bCanApplyDamage = false;
	volatile bool bAttackWindowOpen = true;
	volatile bool bAttackWindowMissed = false;
	float Stamina = 100.f;

	struct FAttackComboNode* ComboNode;
	class ABaseCharacter* LastHitEnemy;

	void ConstructCombos();

protected:
	struct FAttackComboNode* ComboChains;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> AttackMontages;

	// Called when the game starts
	virtual void BeginPlay() override;
	bool IsAttackChainable(EAttackType CurrentAttack);
	void UpdateHUDs(class UAnimMontage* AnimMontage, float AnimTime);
	void ClearHUD();

public:	
	UComboComponent();

	void SetWeapon(class AMeleeWeapon* Weapon);
	void InitiateAttack(EAttackType AttackType);
	void OnAttackStop();
	void OnNextCombo();
	void OnComboReset();
	void SetAttackWindow(bool IsOpen);
	void ApplyStatusToWeapon();
	void MarkLastHitEnemy(class ABaseCharacter* Enemy);
	bool IsLastHitEnemy(class ABaseCharacter* Enemy);
};
