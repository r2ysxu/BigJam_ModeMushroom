// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyController.h"
#include "MeleeEnemy.h"
#include "../../Main/MainCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AMeleeEnemyController::AMeleeEnemyController() {
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyHumanPerception")));
	InitializeAIPerceptionSight();
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMeleeEnemyController::PawnDetected);
}

void AMeleeEnemyController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	Owner = Cast<AMeleeEnemy>(InPawn);
	Owner->SetAiController(this);
	GetWorld()->GetTimerManager().SetTimer(RoutineHandler, this, &AMeleeEnemyController::Routine, 0.05f, true);
}

void AMeleeEnemyController::InitializeAIPerceptionSight() {
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("MeleeEnemySightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

ETeamAttitude::Type AMeleeEnemyController::GetTeamAttitudeTowards(const AActor& Other) const {
	const AMeleeEnemy* mobActor = Cast<AMeleeEnemy>(&Other);
	if (IsValid(mobActor)) {
		return ETeamAttitude::Friendly;
	} else if (IsValid(Cast<AMainCharacter>(&Other))) {
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Neutral;
}

void AMeleeEnemyController::PawnDetected(const TArray<AActor*>& DetectedPawns) {
	for (int i = 0; i < DetectedPawns.Num(); i++) {
		AMainCharacter* mc = Cast<AMainCharacter>(DetectedPawns[i]);
		if (IsValid(mc) && Target == nullptr) {
			Target = mc;
			State = EMeleeEnemyState::VE_Chasing;
			break;
		}
	}
}

void AMeleeEnemyController::RoamTo(FVector Location) {
	if (State == EMeleeEnemyState::VE_Roaming) {
		MoveToLocation(Location);
	}
}

void AMeleeEnemyController::Routine() {
	switch (State) {
	case EMeleeEnemyState::VE_Roaming:
		break;
	case EMeleeEnemyState::VE_Chasing:
		if (Owner->GetIsAttacking() || Owner->GetIsSleeping()) {
			StopMovement();
		} else if (IsValid(Target)) {
			MoveToActor(Target);
		}
		break;
	}
}
