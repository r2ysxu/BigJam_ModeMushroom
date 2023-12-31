// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "../../Characters/BaseCharacter.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

	WeaponFXComponent = CreateDefaultSubobject<UNiagaraComponent>("WeaponFXComponent");
	WeaponFXComponent->SetupAttachment(GetRootComponent());
}

void AMeleeWeapon::ApplyDebuffEnhancement(EStatusDebuffType DebuffType) {
	CurrentEffect = DebuffType;
}

void AMeleeWeapon::MarkLastHitEnemy(ABaseCharacter* Enemy) {
	LastHitEnemy = Enemy;
}

void AMeleeWeapon::ClearLastHitEnemy() {
	LastHitEnemy = nullptr;
}

void AMeleeWeapon::Equip(USceneComponent* Mesh, FName SocketName) {
	bEquipped = true;
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	WeaponFXComponent->ActivateSystem();
}

void AMeleeWeapon::UnEquip(USceneComponent* Mesh, FName SocketName) {
	bEquipped = false;
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	WeaponFXComponent->DeactivateImmediate();
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay() {
	Super::BeginPlay();
	MeleeWeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnWeaponMeleeHit);

	FVector start = WeaponMeshComponent->GetSocketLocation("start");
	FVector end = WeaponMeshComponent->GetSocketLocation("end");
	FVector fxLoc = (start + end) / 2.0;
	FRotator rotation = FRotationMatrix::MakeFromZ(end - start).Rotator();

	WeaponLength = (end - start).Size();

	if (IsValid(WeaponFXSystem)) {
		WeaponFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponFXSystem, WeaponMeshComponent, "NiagaraSystem",
			fxLoc, rotation, EAttachLocation::KeepWorldPosition, false, false, ENCPoolMethod::None, true);
		WeaponFXComponent->SetFloatParameter("trail_width", WeaponLength);
	}
}

void AMeleeWeapon::OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || actor == Owner || !Owner->GetIsAttacking() || !bEquipped) return;
	ABaseCharacter* target = Cast<class ABaseCharacter>(actor);
	if (IsValid(target) && target != LastHitEnemy) {
		LastHitEnemy = target;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%d"), CurrentEffect));
		if (CurrentEffect == EStatusDebuffType::VE_POISON && PoisonSplatterFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PoisonSplatterFX, actor->GetActorLocation());
		else if (CurrentEffect == EStatusDebuffType::VE_TOXIN && ToxinSplatterFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ToxinSplatterFX, actor->GetActorLocation());
		Owner->OnHitTarget(target, BaseDamage, CurrentEffect);
	}
}
