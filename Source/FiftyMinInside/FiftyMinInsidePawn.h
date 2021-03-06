// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FiftyMinInsidePawn.generated.h"

UCLASS(Config = Game)
class AFiftyMinInsidePawn : public APawn
{
	GENERATED_BODY()

		/** StaticMesh component that will be the visuals for our flying pawn */
		UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Armaments)
		TArray<class AWeapon*> WeaponsList;

	UPROPERTY(EditAnywhere, Category = Armaments)
		TArray<class AWeapon*> RocketsList;

	UPROPERTY(EditAnywhere, Category = Armaments)
		class AWeapon* FlareLauncher;

	/** Index of the selected weapon */
	int SelectedWeapon;

	/** Index of the selected rocket */
	int SelectedRocket;

	class AGuidedRocket* GuidedRocket;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armaments)
		TArray<TSubclassOf<AWeapon>> WeaponsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armaments)
		TArray<TSubclassOf<AWeapon>> RocketsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armaments)
		TSubclassOf<AWeapon> FlareClass;

public:
	AFiftyMinInsidePawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

protected:

	virtual void BeginPlay();

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the X axis */
	void MoveForwardInput(float Val);

	/** Bound to the Y axis */
	void MoveRightInput(float Val);

	/** Bound to the Z axis */
	void MoveUpInput(float Val);

	/** Bound to the vertical axis */
	void LookUpInput(float Val);

	/** Bound to the horizontal axis */
	void TurnInput(float Val);

	/** Bound to the z axis */
	void RollInput(float Val);

	void MoveGuidedRight(float Val);

	void MoveGuidedUp(float Val);

	void OnFire();

	void StopFire();

	void OnFireSpecial();

	void StopFireSpecial();

	void OnFireFlare();

	void StopFireFlare();

	void OnNextWeapon();

	void OnPreviousWeapon();

	void OnNextRocket();

	void OnPreviousRocket();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


private:

	/** How quickly forward speed changes */
	UPROPERTY(Category = Movement, EditAnywhere)
		float Acceleration;

	/** How quickly forward speed changes */
	UPROPERTY(Category = Movement, EditAnywhere)
		float DecelerationRate;

	UPROPERTY(Category = Movement, EditAnywhere)
		float GuidedAcceleration;

	UPROPERTY(Category = Movement, EditAnywhere)
		float GuidedForwardSpeed;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Movement, EditAnywhere)
		float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Movement, EditAnywhere)
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Movement, EditAnywhere)
		float MinSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	/** Current guided right speed */
	float CurrentGuidedRightSpeed;

	/** Current guided up speed */
	float CurrentGuidedUpSpeed;

	/** Maximum health*/
	UPROPERTY(EditAnywhere, Category = Health)
		float FullHealth;

	/** Remaining health*/
	UPROPERTY(EditAnywhere, Category = Health)
		float RemainingHealth;

	/** Remaining health*/
	UPROPERTY(EditAnywhere, Category = Health)
		float PercentageHealth;

	/** Remining lives*/
	UPROPERTY(EditAnywhere, Category = Health)
		float RemainingLives;

	/** RocketLauncher that is shooting */
	bool bLeftSpecialShooting;

public:

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current right speed */
	float CurrentRightSpeed;

	/** Current up speed */
	float CurrentUpSpeed;

	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	UFUNCTION(BlueprintCallable, Category = Health)
		FORCEINLINE float GetHealth() const { return PercentageHealth; }

	UFUNCTION(BlueprintCallable, Category = Health)
		FORCEINLINE float GetLives() const { return RemainingLives; }

	UFUNCTION(BlueprintCallable, Category = Weapon)
		FORCEINLINE AWeapon* GetCurrentWeapon() const { return WeaponsList[SelectedWeapon]; }

	UFUNCTION(BlueprintCallable, Category = Weapon)
		FORCEINLINE AWeapon* GetCurrenRocket() const { return RocketsList[SelectedRocket]; }

	UFUNCTION(BlueprintCallable, Category = Health)
		float UpdateHealth(float HealthChange);

	UFUNCTION(BlueprintImplementableEvent, Category = Health)
		void DisplayGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = Health)
		void UpdateFireParticles();

	void SetGuidedRocket(class AGuidedRocket* Rocket);

	/* Collects a weapon from a pick up
	* @WeaponIndex index of the collected weapon
	* @bWeapon if the collected weapon is a classic weapon or a rocket launcher
	*/
	UFUNCTION(BlueprintCallable, Category = Armaments)
	bool CollectWeapon(int WeaponIndex, bool bWeapon);

	UFUNCTION(BlueprintCallable, Category = Armaments)
		int PickWeapon();

	UFUNCTION(BlueprintCallable, Category = Armaments)
		int PickSpecialWeapon();
};
