// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAttackComponent.h"

#include "CoreMinimal.h"
#include "ComboComponent.generated.h"


USTRUCT(BlueprintType)
struct FAttackComboNode {
	GENERATED_BODY()

	struct FAttackComboNode* L = nullptr;
	struct FAttackComboNode* R = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UComboComponent : public UBaseAttackComponent {
	GENERATED_BODY()

private:
	const float StaminaDrainPerAttack = .20f;

	volatile bool bCanApplyDamage = false;
	volatile bool bAttackWindowOpen = true;

	struct FAttackComboNode* ComboNode;

	void ConstructCombos();

protected:
	struct FAttackComboNode* ComboChains;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> AttackMontages;

	// Called when the game starts
	virtual void BeginPlay() override;
	bool IsAttackChainable(EAttackType CurrentAttack);

public:	
	UComboComponent();

	void InitiateAttack(EAttackType AttackType);
	void OnAttackStop();
	void OnNextCombo();
	void OnComboReset();
	void SetAttackWindow(bool IsOpen);
};
