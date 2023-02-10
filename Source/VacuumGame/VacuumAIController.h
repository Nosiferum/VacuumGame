// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VacuumAIController.generated.h"

/**
 * 
 */
UCLASS()
class VACUUMGAME_API AVacuumAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
