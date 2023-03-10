// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class VACUUMGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleHitFX();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void HandleDeath();

private:
	UPROPERTY(EditAnywhere, Category="Combat FX")
	UParticleSystem* DeathParticles;
	UPROPERTY(EditAnywhere, Category="Combat FX")
	UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, Category="Combat FX")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, Category="Combat FX")
	USoundBase* HitSound;

	class UHealthComponent* HealthComponent;
};
