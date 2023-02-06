// Copyright Epic Games, Inc. All Rights Reserved.

#include "VacuumGameGameMode.h"
#include "VacuumGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVacuumGameGameMode::AVacuumGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
