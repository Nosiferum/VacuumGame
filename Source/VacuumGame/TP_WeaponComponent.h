// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class AVacuumGameCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VACUUMGAME_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AVacuumGameProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* VacuumAction;
	
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AVacuumGameCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	
	void Vacuum();
	void VacuumFire();
	void PushBackCanceledVacuumedObjects();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float VacuumingDistance = 300.f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float VacuumRadius = 60.f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float InterpolationSpeed = 10.f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float SuckingValue = 0.2f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float ShrinkingSpeed = 2.f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float ShrinkingValue = 0.1f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float ShrinkingThreshold = 145.f;
	UPROPERTY(EditAnywhere, Category="Vacuuming")
	float DestructionThreshold = 130.f;
	UPROPERTY(EditAnywhere, Category="Vacuuming FX")
	UParticleSystem* VacuumParticles;
	UPROPERTY(EditAnywhere, Category="Firing")
	float ProjectileSpawnLocationCoefficient = 200.f;
	UPROPERTY(EditAnywhere, Category="Firing")
	float ProjectileImpulseCoefficient = 3000.f;
	
	/** The Character holding this weapon*/
	AVacuumGameCharacter* Character;
	UWorld* World;
	FVector Start;
	FVector End;
	TArray<AActor*> Ammo;
};
