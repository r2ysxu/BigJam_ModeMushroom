// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAttackComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DebuffComponent.generated.h"

UENUM(BlueprintType)
enum class EMarkDebuffType : uint8 {
	VE_NONE		UMETA(DisplayName = "None"),
	VE_LL		UMETA(DisplayName = "Infect"),
	VE_LR		UMETA(DisplayName = "Sunder"),
	VE_RL		UMETA(DisplayName = "Spore"),
	VE_RR		UMETA(DisplayName = "Drowsy")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UDebuffComponent : public UActorComponent {
	GENERATED_BODY()

private:
	FTimerHandle DotHandler;
	FTimerHandle OnWakeUpHandler;
	float CurrentDoTDamage = 0;
	const float AdditionalDmg = 0.15;

	bool StatusFusable(EStatusDebuffType Status);
	float ConsumeMarkDebuff();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DotPerInstance = 0.01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalDoTDamage = 0.3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SleepDuration = 1.f;

	class ABaseEnemy* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EStatusDebuffType CurrentStatus = EStatusDebuffType::VE_NONE;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMarkDebuffType CurrentMark = EMarkDebuffType::VE_NONE;

	// Called when the game starts
	virtual void BeginPlay() override;
	void ApplyDoT();
	void OnWakeUp();

public:	
	UDebuffComponent();
	
	float ApplyStatus(EStatusDebuffType Status);
};
