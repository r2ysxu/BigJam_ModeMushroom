// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttackComponent.h"
#include "../../Weapons/Melee/MeleeWeapon.h"


UBaseAttackComponent::UBaseAttackComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseAttackComponent::ApplyStatusToWeapon(EStatusDebuffType Debuff) {
	if (IsValid(Weapon)) {
		Weapon->ApplyDebuffEnhancement(Debuff);
	}
}

void UBaseAttackComponent::BeginPlay() {
	Super::BeginPlay();
}

