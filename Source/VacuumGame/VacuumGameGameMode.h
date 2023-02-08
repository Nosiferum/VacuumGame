// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VacuumGameGameMode.generated.h"

UCLASS(minimalapi)
class AVacuumGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVacuumGameGameMode();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAmmoChanged(int32 Ammo);

	virtual void BeginPlay() override;

private:
	class AVacuumGameCharacter* Character;
};



