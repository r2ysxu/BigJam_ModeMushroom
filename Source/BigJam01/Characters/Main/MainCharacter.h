// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

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
class AMainCharacter : public ACharacter {
	GENERATED_BODY()

private:
	const float ROLL_VELOCITY = 500.f;

	FTimerHandle OnDodgeHandler;
	FTimerHandle OnDodgeStopHandler;

	class UComboComponent* ComboComponet;


protected:
	volatile bool bRollWindowOpen = true;
	volatile bool bRolling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* AttackLMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* AttackRMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* AttackEMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* AttackQMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DodgeMontage;

	void OnDodgeRoll();
	void OnDodgeRollStop();

	virtual void AttackL();
	virtual void AttackR();
	virtual void AttackQ();
	virtual void AttackE();
	virtual void DodgeRoll();
	virtual void FocusEnemy();
	
	// To add mapping context
	virtual void BeginPlay();

public:
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDodgeWindow(bool IsOpen);
	FORCEINLINE bool GetIsDodging() { return bRolling; }
	UFUNCTION(BlueprintCallable) class UComboComponent* GetComboComponent();
};

