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
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
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

	NeighborTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("NeighborTrigger"));
	NeighborTrigger->InitSphereRadius(1000.0f);
	NeighborTrigger->SetupAttachment(PlaneMesh);
	NeighborTrigger->SetCollisionProfileName("Trigger");
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
	//MainWeaponOffset.X += 200.f;
	MainWeaponOffset.Z -= 25.f;
	MainWeapon->SetActorLocation(MainWeaponOffset);


}


float AEnemyPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UpdateHealth(-DamageAmount);
	return DamageAmount;
}

void AEnemyPawn::UpdateHealth(float HealthChange)
{
	RemainingHealth += HealthChange;
	RemainingHealth = FMath::Clamp(RemainingHealth, 0.0f, FullHealth);
	//PercentageHealth = RemainingHealth / FullHealth;
	if (RemainingHealth == 0.0f)
	{
		this->UpdateGameCounter();
		float r = FMath::FRand();
		if (r < 0.33f)
		{
			this->DropAmmo();
		}
		else if (r < 0.66f)
		{
			this->DropHealth();
		}
		else if (r < 0.8f)
		{
			this->DropRocket();
		}
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

void AEnemyPawn::AddNeighbor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) {
		AEnemyPawn* Ally = Cast<AEnemyPawn>(OtherActor);
		if (Ally)
		{
			Neighbors.Add(Ally);
		}

	}

}

void AEnemyPawn::RemoveNeighbor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this) {

		AEnemyPawn* Ally = Cast<AEnemyPawn>(OtherActor);
		if (Ally)
		{
			Neighbors.Remove(Ally);
		}
	}
}

USoundBase* AEnemyPawn::GetHitSound()
{
	return HitSound;
}

bool AEnemyPawn::CanFireTo(AActor* Other)
{
	if (MainWeapon->CanFire())
	{
		FHitResult Hit;
		FCollisionQueryParams CollisionParameters;
		CollisionParameters.AddIgnoredActor(this);
		CollisionParameters.AddIgnoredActor(MainWeapon);

		if (GetWorld()->LineTraceSingleByChannel(Hit, MainWeapon->GetActorLocation(), Other->GetActorLocation(), ECC_Pawn, CollisionParameters))
		{
			if (Hit.GetActor())
			{
				return Hit.GetActor() == Other;
			}
		}
	}
	return false;

}
