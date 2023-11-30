// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "../Characters/BaseCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon() {
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWeapon::SetWielder(ABaseCharacter* Wielder) {
	Owner = Wielder;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay() {
	Super::BeginPlay();
	
}

