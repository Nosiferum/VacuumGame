// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "VacuumGameCharacter.h"
#include "VacuumGameProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		//UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			World->SpawnActor<AVacuumGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::Vacuum()
{
	TArray<FHitResult> OutHits;
	Start = GetComponentLocation();
	End = Start + GetRightVector() * VacuumingDistance;

	if (World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_GameTraceChannel2,
	                               FCollisionShape::MakeSphere(VacuumRadius)))
	{
		for (int i = 0; i < OutHits.Num(); i++)
		{
			AActor* OtherActor = OutHits[i].GetActor();
			FVector OtherActorLocation = OtherActor->GetActorLocation();
			FVector OtherActorScale = OtherActor->GetActorScale();
			FVector MuzzleLocation = Start + FVector(0, 0, 10.f) + GetRightVector() * 50.f;

			float Distance = FVector::Dist(OtherActorLocation, MuzzleLocation);

			FVector NewVacuumLocation = FMath::VInterpTo(OtherActorLocation, MuzzleLocation,
			                                             UGameplayStatics::GetWorldDeltaSeconds(this),
			                                             InterpolationSpeed);

			OtherActor->SetActorLocation(NewVacuumLocation);

			if (Distance <= ShrinkingThreshold)
			{
				FVector NewVacuumScale = FMath::VInterpTo(OtherActorScale,
				                                          FVector(ShrinkingValue, ShrinkingValue, ShrinkingValue),
				                                          UGameplayStatics::GetWorldDeltaSeconds(this),
				                                          InterpolationSpeed * ShrinkingSpeed);

				OtherActor->SetActorScale3D(NewVacuumScale);
			}

			if (Distance <= DestructionThreshold)
			{
				Ammo.Add(OtherActor);
				Character->OnAmmoChanged.ExecuteIfBound(Ammo.Num());

				if (VacuumParticles)
					UGameplayStatics::SpawnEmitterAtLocation(this, VacuumParticles, OtherActorLocation,
					                                         OtherActor->GetActorRotation());

				OtherActor->Destroy();
			}
		}
	}

	//DrawDebugSphere(World, End, VacuumRadius, 10, FColor::Blue, false, 5);
}

void UTP_WeaponComponent::VacuumFire()
{
	if (Ammo.Num() > 0)
	{
		AActor* VacuumedActor = Ammo.Pop();
		/*FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;*/

		Character->OnAmmoChanged.ExecuteIfBound(Ammo.Num());

		AActor* SpawnedActor = World->SpawnActor<AActor>(VacuumedActor->GetClass(),
		                                                 GetComponentLocation() + GetRightVector() *
		                                                 ProjectileSpawnLocationCoefficient,
		                                                 GetComponentRotation());

		SpawnedActor->SetOwner(GetOwner());

		if (SpawnedActor == nullptr)
		{
			Ammo.Add(VacuumedActor);
			Character->OnAmmoChanged.ExecuteIfBound(Ammo.Num());
		}

		else
		{
			UStaticMeshComponent* StaticMeshComponent = SpawnedActor->FindComponentByClass<UStaticMeshComponent>();

			if (StaticMeshComponent)
			{
				StaticMeshComponent->AddImpulse(
					SpawnedActor->GetActorForwardVector() * ProjectileImpulseCoefficient * StaticMeshComponent->
					GetMass());

				if (FireSound)
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation(), 0.4f);

				if (FireAnimation)
				{
					AnimationInstance = Character->GetMesh1P()->GetAnimInstance();

					if (AnimationInstance)
						AnimationInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
	}
}

void UTP_WeaponComponent::PushBackCanceledVacuumedObjects()
{
	TArray<FHitResult> OutHits;
	Start = GetComponentLocation();
	End = Start + GetRightVector() * VacuumingDistance;

	if (World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_GameTraceChannel2,
	                               FCollisionShape::MakeSphere(VacuumRadius)))
	{
		for (int i = 0; i < OutHits.Num(); i++)
		{
			AActor* OtherActor = OutHits[i].GetActor();
			UPrimitiveComponent* OtherActorPrimitiveComponent = OtherActor->FindComponentByClass<UPrimitiveComponent>();

			if (OtherActorPrimitiveComponent)
				OtherActor->SetActorScale3D(FVector(1, 1, 1));
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AVacuumGameCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			// Fire
			//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
			                                   &UTP_WeaponComponent::VacuumFire);
			EnhancedInputComponent->BindAction(VacuumAction, ETriggerEvent::Ongoing, this,
			                                   &UTP_WeaponComponent::Vacuum);
			EnhancedInputComponent->BindAction(VacuumAction, ETriggerEvent::Canceled, this,
			                                   &UTP_WeaponComponent::PushBackCanceledVacuumedObjects);
		}
	}
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
