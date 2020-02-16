// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Weapon.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;

	// Set Health parameters
	FullHealth = 100.0f;
	RemainingHealth = FullHealth;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	FullSpeed = 500.0f;
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector LocalMove = CurrentDirection.GetSafeNormal() * FullSpeed * DeltaTime;
	AddActorLocalOffset(LocalMove, true);

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MainWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!MainWeapon)
		MainWeapon = CreateDefaultSubobject<AWeapon>(TEXT("Weapon"));
	MainWeapon->AttachToComponent(PlaneMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	FVector MainWeaponOffset = MainWeapon->GetActorLocation();
	MainWeaponOffset.X += 200.f;
	MainWeaponOffset.Z -= 25.f;
	MainWeapon->SetActorLocation(MainWeaponOffset);
}


float AEnemyPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UpdateHealth(-DamageAmount);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy new health %f "), RemainingHealth));
	return DamageAmount;
}

void AEnemyPawn::UpdateHealth(float HealthChange)
{
	RemainingHealth += HealthChange;
	RemainingHealth = FMath::Clamp(RemainingHealth, 0.0f, FullHealth);
	//PercentageHealth = RemainingHealth / FullHealth;
	if (RemainingHealth == 0.0f)
	{
		MainWeapon->Destroy();
		Destroy();
	}
}

void AEnemyPawn::OnFire()
{
	MainWeapon->Fire();
}

void AEnemyPawn::StopFire()
{
	MainWeapon->StopFire();
}

