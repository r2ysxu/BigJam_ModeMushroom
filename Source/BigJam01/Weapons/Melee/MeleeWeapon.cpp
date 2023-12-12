// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "../../Characters/BaseCharacter.h"

#include "Components/BoxComponent.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	class USceneComponent* sceneRoot = CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot");
	SetRootComponent(sceneRoot);

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("OneHandWeaponMesh");
	WeaponMeshComponent->SetupAttachment(GetRootComponent());
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeleeWeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeWeaponBox"));
	MeleeWeaponBox->SetBoxExtent(FVector(5.f, 5.f, 100.f));
	MeleeWeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeleeWeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	MeleeWeaponBox->SetupAttachment(GetRootComponent());
	//MeleeWeaponBox->bHiddenInGame = false;
}

void AMeleeWeapon::ApplyDebuffEnhancement(EStatusDebuffType DebuffType) {
	CurrentEffect = DebuffType;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay() {
	Super::BeginPlay();
	MeleeWeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnWeaponMeleeHit);
}

void AMeleeWeapon::OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || actor == Owner) return;
	Owner->OnHitTarget(Cast<class ABaseCharacter>(actor), BaseDamage, CurrentEffect);
}
