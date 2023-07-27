// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPawn.h"
#include "../DataTable/DT_Struct.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadonly, Category = "BP_Pawn")
		AMyPawn* BP_Pawn;

	UFUNCTION(BlueprintCallable)
		void CameraMove(FVector location, float y, float z, float nextSpringArmLength, float minSpringArmLength, float maxSpringArmLength);
protected:
	virtual void BeginPlay() override;
};
