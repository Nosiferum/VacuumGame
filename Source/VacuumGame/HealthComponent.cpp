// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "EnemyCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Owner = GetOwner();
	EnemyCharacter = Cast<AEnemyCharacter>(Owner);

	if (Owner)
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                   AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	Health -= Damage;

	if (EnemyCharacter)
		EnemyCharacter->OnHealthChanged();

	if (Health <= 0.f)
	{
		OnActorDied.Broadcast();
		Owner->Destroy();
	}
}
