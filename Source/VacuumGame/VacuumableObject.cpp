// Fill out your copyright notice in the Description page of Project Settings.


#include "VacuumableObject.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVacuumableObject::AVacuumableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AVacuumableObject::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AVacuumableObject::OnHit);
}

void AVacuumableObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	const AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr) return;

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	const TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OtherActor->ActorHasTag("Enemy"))
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
}

// Called every frame
void AVacuumableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

