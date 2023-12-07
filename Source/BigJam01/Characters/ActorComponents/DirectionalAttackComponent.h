// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAttackComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DirectionalAttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackSwingDirection : uint8 {
	VE_F UMETA(DisplayName = "Front"),
	VE_L UMETA(DisplayName = "Left"),
	VE_R UMETA(DisplayName = "Right")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UDirectionalAttackComponent : public UBaseAttackComponent {
	GENERATED_BODY()

private:
	const float StaminaDrainPerAttack = .20f;

	int CurrentAttackIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> FrontAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> LeftAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> RightAttackMontages;

	EAttackSwingDirection LastMovement = EAttackSwingDirection::VE_F;
	volatile bool bAttackWindowOpen = true;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UDirectionalAttackComponent();

	void SetDirectionalMovement(FVector2D MovementVector);
	bool IsAttackChainable(EAttackType CurrentAttack);
	void InitiateAttack(EAttackType AttackType);
	void SetAttackWindow(bool IsOpen);
	void OnNextCombo();
	void OnComboReset();
	void OnAttackStop();
};
