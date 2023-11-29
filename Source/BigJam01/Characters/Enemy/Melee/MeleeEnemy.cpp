// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"
#include "../../Main/MainCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMeleeEnemy::AMeleeEnemy() {
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	MeleeDetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeDetectionBox"));
	MeleeDetectionComponent->SetSphereRadius(50.f);
	MeleeDetectionComponent->AddLocalOffset(FVector(60.f, 0.f, 0.f));
	MeleeDetectionComponent->SetupAttachment(GetRootComponent());
	MeleeDetectionComponent->bHiddenInGame = false;

	MeleeWeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeWeaponBox"));
	MeleeWeaponBox->SetBoxExtent(FVector(5.f, 5.f, 50.f));
	MeleeWeaponBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketL);
	MeleeWeaponBox->bHiddenInGame = false;
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
	if (!bAttacking) {
		bAttacking = true;
		PerformAttack();
	}
}

void AMeleeEnemy::PerformAttack() {
	SelectedChain = FMath::RandRange(0, 0);
	AttackChain();
}

void AMeleeEnemy::AttackChain() {
	if (SelectedChain < AttackChains.Num()) {
		bAttackSwing = false;
		FAttackChain* AttackChain = &AttackChains[SelectedChain];
		if (AttackIndex >= AttackChain->Attacks.Num()) {
			ClearAttacks();
		} else {
			SingleAttack(AttackChain->Attacks[AttackIndex++]);
		}
	}
}

void AMeleeEnemy::OnFlinchRecover() {
	bFlinching = false;
	GetWorld()->GetTimerManager().PauseTimer(InitiateAttackHandler);
}

void AMeleeEnemy::SingleAttack(EMeleeAttackType AttackType) {
	if (AttackMontages[(uint8)AttackType]) {
		bAttackSwing = true;
		float animationDelay = PlayAnimMontage(AttackMontages[(uint8)AttackType]);
		GetWorld()->GetTimerManager().SetTimer(SingleAttackHandler, this, &AMeleeEnemy::AttackChain, animationDelay, false);
	}
}

void AMeleeEnemy::OnWithinMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || bFlinching) return;
	AMainCharacter* mc = Cast<AMainCharacter>(actor);
	if (IsValid(mc)) {
		GetWorld()->GetTimerManager().SetTimer(InitiateAttackHandler, this, &AMeleeEnemy::InitiateMeleeAttack, ATTACK_DELAY, true, INITIAL_ATTACK_DELAY);
	}
}

void AMeleeEnemy::OnOutsideMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (actor == this) return;
	GetWorld()->GetTimerManager().ClearTimer(InitiateAttackHandler);
}

void AMeleeEnemy::OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || !bAttacking || !bAttackSwing) return;
	AMainCharacter* mc = Cast<AMainCharacter>(actor);
	if (IsValid(mc)) {
		mc->OnHitByOpponent();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Hit"));
		bAttackSwing = false;
	}
}

void AMeleeEnemy::ClearAttacks() {
	AttackIndex = 0;
	bAttacking = false;
	GetWorld()->GetTimerManager().ClearTimer(SingleAttackHandler);
}

void AMeleeEnemy::OnHitByOpponent() {
	ClearAttacks();
	if (FlinchMontage) {
		float animationDelay = PlayAnimMontage(FlinchMontage);
		GetWorld()->GetTimerManager().PauseTimer(InitiateAttackHandler);
		GetWorld()->GetTimerManager().SetTimer(FlinchHandler, this, &AMeleeEnemy::OnFlinchRecover, animationDelay, false);
	}
}