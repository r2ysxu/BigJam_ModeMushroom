// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAttackComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChargeAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UChargeAttackComponent : public UBaseAttackComponent {
	GENERATED_BODY()

private:
	class AMainCharacter* Owner;

protected:
	const float StaminaDrainPerAttack = 0.25f;

	FTimerHandle OnChargeUpHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> AttackMontages;

	volatile int ComboChain = 0;
	volatile bool bAttackWindowOpen = true;

	virtual void BeginPlay() override;

public:
	UChargeAttackComponent();
	
	bool IsAttackChainable(EAttackType CurrentAttack);
	void InitiateAttack(EAttackType AttackType);
	void OnAttackStop();
	void OnNextCombo();
	void OnComboReset();
	bool OnChargedUp();
	void SetAttackWindow(bool IsOpen);
};
