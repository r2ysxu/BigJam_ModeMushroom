// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "../ActorComponents/BaseAttackComponent.h"
#include "../ActorComponents/DebuffComponent.h"
#include "../../Widgets/HUDs/EnemyHud.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DebuffComponent = CreateDefaultSubobject<UDebuffComponent>(TEXT("DebuffComponent"));
	EnemyHudComponet = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHUD"));
	EnemyHudComponet->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay() {
	Super::BeginPlay();
	EnemyHud = CreateWidget<UEnemyHUD>(GetWorld(), EnemyHudClass);
	if (EnemyHud) {
		EnemyHud->SetEnemy(this);
		EnemyHudComponet->SetWidget(EnemyHud);
		EnemyHudComponet->SetVisibility(false);
	}
}

bool ABaseEnemy::CheckAlive() {
	if (Health <= 0) {
		bAlive = false;
	}
	return Health > 0;
}

void ABaseEnemy::TakeHitDamage(float Damage) {
	bSleeping = false;
	Health -= FMath::Min(Health, Damage);
}

float ABaseEnemy::OnHitByOpponent(float Damage, EStatusDebuffType Status) {
	TakeHitDamage(Damage);
	return DebuffComponent->ApplyStatus(Status);
}

void ABaseEnemy::SetIsSleeping(bool IsSleeping) {
	bSleeping = IsSleeping;
}

void ABaseEnemy::ShowHud() {
	EnemyHudComponet->SetVisibility(true);
	GetWorld()->GetTimerManager().SetTimer(UpdateHudHander, this, &ABaseEnemy::UpdateHud, 0.1f, true);
}

void ABaseEnemy::UpdateHud() {
	FVector cameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	EnemyHudComponet->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(EnemyHudComponet->GetComponentLocation(), cameraLocation));
}

bool ABaseEnemy::GetInAnimLock() {
	return bInAnimLock;
}

void ABaseEnemy::SetInAnimLock(bool bLock) {
	bInAnimLock = bLock;
}
