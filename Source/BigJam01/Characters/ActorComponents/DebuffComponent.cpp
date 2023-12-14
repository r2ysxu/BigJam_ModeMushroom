// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffComponent.h"
#include "../Enemy/BaseEnemy.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UDebuffComponent::UDebuffComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

float UDebuffComponent::ApplyStatus(EStatusDebuffType Status) {
	if (Status == EStatusDebuffType::VE_NONE) return 0.f;
	else if (StatusFusable(Status)) return 0.f;
	else if (Status == EStatusDebuffType::VE_ZMD) return ConsumeMarkDebuff();
	else return 0.f;
}


bool UDebuffComponent::StatusFusable(EStatusDebuffType Status) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("%d"), Status));
	if (Status == EStatusDebuffType::VE_NONE || Status == EStatusDebuffType::VE_ZMD) {
		return false;
	} else if (CurrentStatus == EStatusDebuffType::VE_NONE) {
		CurrentStatus = Status;
		return true;
	} else if (CurrentStatus == EStatusDebuffType::VE_POISON) {
		switch (Status) {
		case EStatusDebuffType::VE_POISON: CurrentMark = EMarkDebuffType::VE_LL; break;
		case EStatusDebuffType::VE_TOXIN: CurrentMark = EMarkDebuffType::VE_LR; break;
		}
		CurrentStatus = EStatusDebuffType::VE_NONE;
		return true;
	} else if (CurrentStatus == EStatusDebuffType::VE_TOXIN) {
		switch (Status) {
		case EStatusDebuffType::VE_POISON: CurrentMark = EMarkDebuffType::VE_RL; break;
		case EStatusDebuffType::VE_TOXIN: CurrentMark = EMarkDebuffType::VE_RR; break;
		}
		CurrentStatus = EStatusDebuffType::VE_NONE;
		return true;
	} else {
		return false;
	}
}

float UDebuffComponent::ConsumeMarkDebuff() {
	switch (CurrentMark) {
	case EMarkDebuffType::VE_NONE: break;
	case EMarkDebuffType::VE_LL: 
		CurrentDoTDamage = TotalDoTDamage;
		GetWorld()->GetTimerManager().SetTimer(DotHandler, this, &UDebuffComponent::ApplyDoT, 0.5f, true);
		if (ProcSplatterFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProcSplatterFX, Owner->GetActorLocation());
		CurrentMark = EMarkDebuffType::VE_NONE;
		break;
	case EMarkDebuffType::VE_LR:
		Owner->TakeHitDamage(AdditionalDmg);
		CurrentMark = EMarkDebuffType::VE_NONE;
		break;
	case EMarkDebuffType::VE_RL:
		return AdditionalDmg;
		CurrentMark = EMarkDebuffType::VE_NONE;
		break;
	case EMarkDebuffType::VE_RR:
		Owner->SetIsSleeping(true);
		GetWorld()->GetTimerManager().SetTimer(OnWakeUpHandler, this, &UDebuffComponent::OnWakeUp, SleepDuration, false);
		CurrentMark = EMarkDebuffType::VE_NONE;
		break;
	}
	return 0.f;
}

// Called when the game starts
void UDebuffComponent::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<ABaseEnemy>(GetOwner());
}

void UDebuffComponent::ApplyDoT() {
	if (CurrentDoTDamage <= 0) {
		GetWorld()->GetTimerManager().ClearTimer(DotHandler);
	} else {
		Owner->TakeHitDamage(DotPerInstance);
	}
}

void UDebuffComponent::OnWakeUp() {
	Owner->SetIsSleeping(false);
}
