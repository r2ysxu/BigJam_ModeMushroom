// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"
#include "../../Main/MainCharacter.h"

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
	MeleeDetectionComponent->AddLocalOffset(FVector(50.f, 0.f, 0.f));
	MeleeDetectionComponent->SetupAttachment(GetRootComponent());
	MeleeDetectionComponent->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void AMeleeEnemy::BeginPlay() {
	Super::BeginPlay();
	SpawnDefaultController();
	MeleeDetectionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemy::OnWithinMeleeRange);
	MeleeDetectionComponent->OnComponentEndOverlap.AddDynamic(this, &AMeleeEnemy::OnOutsideMeleeRange);
}

void AMeleeEnemy::InitiateMeleeAttack() {
	if (!bAttacking) {
		bAttacking = true;
		PerformAttack();
	}
}

void AMeleeEnemy::PerformAttack() {
	SelectedChain = FMath::RandRange(0, 0);
	if (SelectedChain < AttackChains.Num()) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("AttackChain"));
		AttackChain();
	}
}

void AMeleeEnemy::AttackChain() {
	FAttackChain* AttackChain = &AttackChains[SelectedChain];
	if (AttackIndex >= AttackChain->Attacks.Num()) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("AttackEnd"));
		AttackIndex = 0;
		bAttacking = false;
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Attack"));
		EMeleeAttackType currentAttack = AttackChain->Attacks[AttackIndex++];
		float animationDelay = SingleAttack(AttackMontages[(uint8)currentAttack]);
		GetWorld()->GetTimerManager().SetTimer(SingleAttackHandler, this, &AMeleeEnemy::AttackChain, animationDelay, false);
	}
}

float AMeleeEnemy::SingleAttack(UAnimMontage* AttackMontage) {
	if (AttackMontage) {
		return PlayAnimMontage(AttackMontage);
	}
	return 0.f;
}

void AMeleeEnemy::OnWithinMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this) return;
	AMainCharacter* mc = Cast<AMainCharacter>(actor);
	if (IsValid(mc)) {
		GetWorld()->GetTimerManager().SetTimer(InitiateAttackHandler, this, &AMeleeEnemy::InitiateMeleeAttack, ATTACK_DELAY, true, INITIAL_ATTACK_DELAY);
	}
}

void AMeleeEnemy::OnOutsideMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	GetWorld()->GetTimerManager().ClearTimer(InitiateAttackHandler);
}
