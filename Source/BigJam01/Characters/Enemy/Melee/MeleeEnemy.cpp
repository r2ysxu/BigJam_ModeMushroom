// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"
#include "../../Main/MainCharacter.h"
#include "../../ActorComponents/DebuffComponent.h"
#include "MeleeEnemyController.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMeleeEnemy::AMeleeEnemy() {
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = RoamSpeed;

	MeleeDetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeDetectionBox"));
	MeleeDetectionComponent->SetSphereRadius(50.f);
	MeleeDetectionComponent->AddLocalOffset(FVector(60.f, 0.f, 0.f));
	MeleeDetectionComponent->SetupAttachment(GetRootComponent());
	MeleeDetectionComponent->bHiddenInGame = true;

	MeleeWeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeWeaponBox"));
	MeleeWeaponBox->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	MeleeWeaponBox->SetupAttachment(GetMesh(), WeaponSocket);

}

void AMeleeEnemy::SetAiController(AMeleeEnemyController* MEController) {
	AiController = MEController;
}

bool AMeleeEnemy::CheckAlive() {
	if (Health <= 0) {
		ClearAttacks();
		AiController->NotifyDead();
		if (IsValid(EnemyHud)) {
			EnemyHud->SetVisibility(ESlateVisibility::Hidden);
			EnemyHud->RemoveFromParent();
		}
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		GetMovementComponent()->Deactivate();
		GetCapsuleComponent()->DestroyComponent();
	}
	return Super::CheckAlive();
}

void AMeleeEnemy::NextMoveLocation(FVector Path) {
	AiController->RoamTo(Path);
}

// Called when the game starts or when spawned
void AMeleeEnemy::BeginPlay() {
	Super::BeginPlay();
	SpawnDefaultController();
	MeleeDetectionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemy::OnWithinMeleeRange);
	MeleeDetectionComponent->OnComponentEndOverlap.AddDynamic(this, &AMeleeEnemy::OnOutsideMeleeRange);
	MeleeWeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemy::OnWeaponMeleeHit);
}

void AMeleeEnemy::InitiateMeleeAttack() {
	if (!bAttacking and !bInAnimLock) {
		bAttacking = true;
		PerformAttack();
	}
}

void AMeleeEnemy::PerformAttack() {
	SelectedChain = FMath::RandRange(0, AttackChains.Num() - 1);
	AttackChain();
}

void AMeleeEnemy::AttackChain() {
	if (SelectedChain < AttackChains.Num()) {
		bAttackSwing = false;
		FAttackChain* AttackChain = &AttackChains[SelectedChain];
		if (AttackIndex >= AttackChain->Attacks.Num()) {
			ClearAttacks();
			GetWorld()->GetTimerManager().SetTimer(InitiateAttackHandler, this, &AMeleeEnemy::InitiateMeleeAttack, RecoveryDelay, false);
		} else {
			SingleAttack(AttackChain->Attacks[AttackIndex++]);
		}
	}
}

void AMeleeEnemy::OnFlinchRecover() {
	bFlinching = false;
	GetWorld()->GetTimerManager().ClearTimer(SingleAttackHandler);
	InitiateMeleeAttack();
}

void AMeleeEnemy::OnDashStop() {
	bDashing = false;
}

void AMeleeEnemy::SingleAttack(EMeleeAttackType AttackType) {
	if (AttackMontages[(uint8)AttackType] && !bInAnimLock) {
		bAttackSwing = true;
		float animationDelay = PlayAnimMontage(AttackMontages[(uint8)AttackType]);
		GetWorld()->GetTimerManager().SetTimer(SingleAttackHandler, this, &AMeleeEnemy::AttackChain, animationDelay, false);
	}
}

void AMeleeEnemy::OnWithinMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || bFlinching) return;
	AMainCharacter* mc = Cast<AMainCharacter>(actor);
	if (IsValid(mc)) {
		bMeleeRange = true;
		InitiateMeleeAttack();
	}
}

void AMeleeEnemy::OnOutsideMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (actor == this) return;
	bMeleeRange = false;
	ClearAttacks();
	GetWorld()->GetTimerManager().ClearTimer(InitiateAttackHandler);
}

void AMeleeEnemy::OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || !bAttacking || !bAttackSwing) return;
	AMainCharacter* mc = Cast<AMainCharacter>(actor);
	if (IsValid(mc)) {
		mc->OnHitByOpponent(DamagePerHit, EStatusDebuffType::VE_NONE);
		bAttackSwing = false;
	}
}

void AMeleeEnemy::ClearAttacks() {
	AttackIndex = 0;
	bAttacking = false;
	GetWorld()->GetTimerManager().ClearTimer(SingleAttackHandler);
}


float AMeleeEnemy::OnHitByOpponent(float Damage, EStatusDebuffType Status) {
	ClearAttacks();
	if (FlinchMontage) {
		float animationDelay = PlayAnimMontage(FlinchMontage);
		GetWorld()->GetTimerManager().SetTimer(FlinchHandler, this, &AMeleeEnemy::OnFlinchRecover, animationDelay, false);
	}
	return Super::OnHitByOpponent(Damage, Status);
}

void AMeleeEnemy::DashForward() {
	if (DashFwdMontage && !bInAnimLock) {
		bDashing = true;
		float animationDelay = PlayAnimMontage(DashFwdMontage);
		GetWorld()->GetTimerManager().SetTimer(DashHandler, this, &AMeleeEnemy::OnDashStop, animationDelay, false);
	}
}

void AMeleeEnemy::DashBack() {
	if (DashBackMontage && !bInAnimLock && FMath::SRand() >= ( 1.f - DodgeChance)) {
		bDashing = true;
		float animationDelay = PlayAnimMontage(DashBackMontage);
		GetWorld()->GetTimerManager().SetTimer(DashHandler, this, &AMeleeEnemy::OnDashStop, animationDelay, false);
	}
}

bool AMeleeEnemy::GetIsDashing() {
	return bDashing;
}

void AMeleeEnemy::OnStartChasing() {
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	ShowHud();
}

bool AMeleeEnemy::GetIsMeleeRange() {
	return bMeleeRange;
}

bool AMeleeEnemy::HasOverlappingActors() {
	TArray<AActor*> result;
	MeleeDetectionComponent->GetOverlappingActors(result, AMainCharacter::StaticClass());
	return !result.IsEmpty();
}
