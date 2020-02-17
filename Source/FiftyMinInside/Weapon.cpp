#include "Weapon.h"
#include "Bullet.h"
#include "Engine/World.h"
#include "TimerManager.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	bTryFire = false;
	LastFire = 0.0f;
	DelayBetweenShots = 0.4f;

	bUseMunition = false;
	MunitionCount = 5;
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	LastFire = DelayBetweenShots;
}


void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AWeapon::GetMunitionCount()
{
	return MunitionCount;
}

bool AWeapon::CanFire() {
	return !bTryFire;
}

void AWeapon::FireProjectile()
{
	/*if (BulletClass)
	{
		if (bUseMunition)
		{
			if (MunitionCount == 0) {
				return;
			}
			--MunitionCount;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Pew!"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;

		FVector Offset = GetActorLocation();
		if (bAltern)
		{
			Offset.Y += (bFireLeft) ? -70.f : 70.f;
			bFireLeft = !bFireLeft;
		}
		

		GetWorld()->SpawnActor <ABullet>(BulletClass, BulletTransform, SpawnParams);
	}

	*/
}



void AWeapon::Fire() {
	if (CanFire()) {
		bTryFire = true;
		float InGameTime = GetWorld()->GetTimeSeconds();
		if (LastFire + DelayBetweenShots > InGameTime)
		{
			//Delay shot
			GetWorld()->GetTimerManager().SetTimer(TimeHandleFiring, this, &AWeapon::FireProjectile, LastFire + DelayBetweenShots - InGameTime, false);
		}
		else
		{
			FireProjectile();
		}

	}
}



void AWeapon::StopFire() {
	bTryFire = false;
	GetWorldTimerManager().ClearTimer(TimeHandleFiring);

}



