// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API AMeleeEnemyController : public AAIController {
	GENERATED_BODY()

private:
	FTimerHandle RoutineHandler;

	void InitializeAIPerceptionSight();

protected:
	class AMeleeEnemy* Owner;
	class AMainCharacter* Target = nullptr;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	AMeleeEnemyController();
	virtual void OnPossess(APawn* InPawn) override;



	UFUNCTION() void PawnDetected(const TArray<AActor*>& DetectedPawns);

	void Routine();
};
