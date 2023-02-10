// Fill out your copyright notice in the Description page of Project Settings.


#include "VacuumAIController.h"

#include "Kismet/GameplayStatics.h"

void AVacuumAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	SetFocus(PlayerPawn);
}
