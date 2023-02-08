// Copyright Epic Games, Inc. All Rights Reserved.

#include "VacuumGameGameMode.h"

#include "VacuumGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AVacuumGameGameMode::AVacuumGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	

}

void AVacuumGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AVacuumGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Character)
	{
		UE_LOG(LogTemp, Display, TEXT("bind"));
		//WeaponComponent->OnAmmoChanged.BindDynamic(this, &AVacuumGameGameMode::HandleAmmo);
		Character->OnAmmoChanged.BindDynamic(this, &AVacuumGameGameMode::OnAmmoChanged);
	}

	else
	{
		UE_LOG(LogTemp, Display, TEXT("No bind"));
	}
}

