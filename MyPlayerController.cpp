// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::CameraMove(FVector location, float y, float z, float nextSpringArmLength, float minSpringArmLength, float maxSpringArmLength)
{

	BP_Pawn->newLocation = location;
	BP_Pawn->newPitch = y;
	BP_Pawn->newYaw = z;
	BP_Pawn->currentSpringArmLength = nextSpringArmLength;
	BP_Pawn->minSpringArmLength = minSpringArmLength;
	BP_Pawn->maxSpringArmLength = maxSpringArmLength;
	BP_Pawn->scaleSpeed = BP_Pawn->maxSpringArmLength / 2;
	BP_Pawn->Focus();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(true);
	BP_Pawn = Cast<AMyPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	CameraMove(FVector(0.0f,0.0f, 0.0f), -35.0f, -45.0f, 7000, 2000, 20000);
}
