// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FIFTYMININSIDE_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UProjectileMovementComponent* BulletMovement;

	UPROPERTY(EditAnywhere, Category = "Object")
		float Speed;

	UPROPERTY(EditAnywhere, Category = Damage)
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = Damage)
		TSubclassOf<class ABulletExplosion> BulletExplosion;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UMaterial* Material;

	UPROPERTY(EditAnywhere, Category = "Components")
		bool bSticky;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnBulletHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

};
