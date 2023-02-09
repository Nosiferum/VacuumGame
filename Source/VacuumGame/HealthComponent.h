// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VACUUMGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	/*UPROPERTY(BlueprintCallable, BlueprintAssignable, VisibleAnywhere)
	FOnHealthChanged OnHealthChanged;*/
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);
	
private:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float Health = 0.f;

	//class UWidgetComponent* HealthBar;
	AActor* Owner;
	class AEnemyCharacter* EnemyCharacter;
};
