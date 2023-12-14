// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeEnemyController.generated.h"

UENUM(BlueprintType)
enum class EMeleeEnemyState : uint8 {
	VE_Roaming	 UMETA(DisplayName = "Roaming"),
	VE_Chasing	 UMETA(DisplayName = "Chasing"),
	VE_Attacking UMETA(DisplayName = "Attacking"),
	VE_Recovery	 UMETA(DisplayName = "Recovery"),
};

/**
 * 
 */
UCLASS()
class BIGJAM01_API AMeleeEnemyController : public AAIController {
	GENERATED_BODY()

private:
	const float SightRange = 1000.f;
	const float DashCooldownRate = 2.f;

	FTimerHandle RoutineHandler;
	FTimerHandle DashCooldownHandler;

	void InitializeAIPerceptionSight();

protected:
	class AMeleeEnemy* Owner;
	class AMainCharacter* Target = nullptr;
	EMeleeEnemyState State = EMeleeEnemyState::VE_Roaming;
	bool bDashCoolDown = false;

	void OnDashCooldown();
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	AMeleeEnemyController();
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION() void PawnDetected(const TArray<AActor*>& DetectedPawns);

	void RoamTo(FVector Location);
	void Routine();
};
