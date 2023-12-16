// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyController.h"
#include "MeleeEnemy.h"
#include "../../Main/MainCharacter.h"
#include "../../ActorComponents/EnemyReactionComponent.h"

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
	GetWorld()->GetTimerManager().SetTimer(RoutineHandler, this, &AMeleeEnemyController::Routine, 0.1f, true);
}

void AMeleeEnemyController::InitializeAIPerceptionSight() {
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("MeleeEnemySightConfig"));
	SightConfig->SightRadius = SightRange;
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

void AMeleeEnemyController::OnDashCooldown() {
	bDashCoolDown = false;
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
			Target->GetEnemyReactionComponent()->Subscribe(Owner);
			Owner->OnStartChasing();
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
			}
			else if (Owner->HasOverlappingActors()) {
				State = EMeleeEnemyState::VE_Attacking;
			}
			else if (IsValid(Target) && !Owner->GetIsDashing() && !Owner->GetInAnimLock()) {
				float distance = Owner->GetDistanceTo(Target);
				float dashChance = FMath::SRand() + distance / SightRange;
				if (!bDashCoolDown && dashChance > 0.75) {
					MoveToActor(Target);
					bDashCoolDown = true;
					Owner->DashForward();
					if (!bDashCoolDown) GetWorld()->GetTimerManager().SetTimer(DashCooldownHandler, this, &AMeleeEnemyController::OnDashCooldown, DashCooldownRate, false);
				} else {
					MoveToActor(Target);
				}
			}
			break;
		case EMeleeEnemyState::VE_Attacking:
			MoveToActor(Target);
			Owner->InitiateMeleeAttack();
			if (!Owner->GetIsMeleeRange() || !Owner->HasOverlappingActors()) {
				State = EMeleeEnemyState::VE_Chasing;
			}
			break;
	}

}

void AMeleeEnemyController::NotifyDead() {
	if (IsValid(Target)) {
		Target->GetEnemyReactionComponent()->Unsubscribe(Owner);
	}
}
