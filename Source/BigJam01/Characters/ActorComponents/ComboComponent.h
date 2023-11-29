// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Components/ActorComponent.h"
#include "ComboComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	VE_L UMETA(DisplayName = "Attack_L"),
	VE_R UMETA(DisplayName = "Attack_R"),
	VE_Q UMETA(DisplayName = "Attack_Q"),
	VE_E UMETA(DisplayName = "Attack_E")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UComboComponent : public UActorComponent {
	GENERATED_BODY()

private:
	const int32 MAX_CHAIN_COMBO = 1;
	class AMainCharacter* Owner;
	FTimerHandle OnAttackHandler;
	TArray<EAttackType> PreviousAttacks;

	volatile bool bCanApplyDamage = false;
	volatile bool bAttackWindowOpen = true;

	class ABaseEnemy* LastHitEnemy;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool IsAttackChainable(EAttackType CurrentAttack);
	void ApplyStatus();

public:	
	UComboComponent();

	void InitiateAttack(class UAnimMontage* AttackMontage, EAttackType AttackType);
	void OnAttackStop();
	void OnNextCombo();
	void OnComboReset();
	void SetAttackWindow(bool IsOpen);
	void MarkLastHitEnemy(class ABaseEnemy* Enemy);
	bool IsLastHitEnemy(class ABaseEnemy* Enemy);
};
