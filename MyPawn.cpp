// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//create and set  the springArm as the RootComponent
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	cameraSpringArm->SetupAttachment(RootComponent);

	//set springArm rotation
	cameraSpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));

	//set springArm is smooth(平滑(移动、旋转))
	cameraSpringArm->bEnableCameraLag = true;
	cameraSpringArm->bEnableCameraRotationLag = true;

	//create and set the camera as the cameraSpringArm
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(cameraSpringArm);
	
	//设置优先级
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

void AMyPawn::Focus()
{
	currentLocation = GetActorLocation();
	currentPitch = GetControlRotation().Pitch;
	currentYaw = GetControlRotation().Yaw;
	timeline.SetPlayRate(focusAnimationRate);

	if (timelineCurve)
	{
		timeline.PlayFromStart();
	}
}

void AMyPawn::FocusMov(float lerp)
{
	targetLocation = FMath::Lerp(currentLocation, newLocation, lerp);
	targetRotation = FMath::Lerp(FRotator(currentPitch, currentYaw, 0.0f), FRotator(newPitch, newYaw, 0.0f), lerp);
	SetActorLocation(targetLocation);
	playerController->SetControlRotation(targetRotation);
}

void AMyPawn::SpeedUp(float DeltaTime)
{
	if (speedIsUp)
	{
		AccelerationFactor += DeltaTime;
	}
	else
	{
		AccelerationFactor -= DeltaTime;
	}
	AccelerationFactor = FMath::Clamp<float>(AccelerationFactor, 1.0f, maxAccelerationFactor);
	if (AccelerationFactor >= maxAccelerationFactor || AccelerationFactor <= 1.0f)
	{
		isAcceleration = false;
	}
}

float AMyPawn::GetSpringArmLenNoramlized()
{
	float value = 0;
	value = (cameraSpringArm->TargetArmLength - minSpringArmLength) / (maxSpringArmLength - minSpringArmLength);
	return value;
}

void AMyPawn::CanMove()
{
	isTurn = true;
}

void AMyPawn::CanNotMove()
{
	isTurn = false;
}


//move left and right
void AMyPawn::MoveRight(float v)
{
	if (isLock == false) return;
	if (v == 0.0f) return;
	FVector moveVector;
	moveVector.Y = v * moveSpeed * AccelerationFactor;
	AddActorLocalOffset(moveVector);

}

//Forward and backward movement
void AMyPawn::MoveForWorld(float v)
{
	if (isLock == false) return;
	if (v == 0.0f) return;
	FVector moveVector;
	moveVector.X = v * moveSpeed * AccelerationFactor;;
	AddActorLocalOffset(moveVector);
}

//move up and down
void AMyPawn::MoveUp(float v)
{
	if (isLock == false) return;
	if (v == 0.0f) return;
	FVector moveVector;
	moveVector.Z = v * moveSpeed * AccelerationFactor;;
	AddActorWorldOffset(moveVector);
}

//rotate left and right
void AMyPawn::TurnRight(float v)
{
	if (isLock == false) return;
	if (isTurn == false) return;
	if (v == 0.0f) return;
	v *= rotationSpeed;
	cameraSpringArm->AddRelativeRotation(FRotator(0.0f, v, 0.0f));
}

//rotate up and down
void AMyPawn::TurnUp(float v)
{
	if (isLock == false) return;
	if (isTurn == false) return;
	if (v == 0.0f) return;
	v *= rotationSpeed;
	add += v;
	add = FMath::Clamp<float>(add, minAngle, mmaxAngle);
	FRotator rotator = cameraSpringArm->GetRelativeRotation();
	cameraSpringArm->SetRelativeRotation(FRotator(add, rotator.Yaw, 0.0f));
}

//缩放
void AMyPawn::Zoom(float v)
{
	float value = 0.0f;
	if (isLock == false) return;
	if (v == 0.0f) return;
	//v *= -scaleSpeed;
	if (v >= 0 )
	{
		value = cameraSpringArm->TargetArmLength - (timelineCurve->GetFloatValue(GetSpringArmLenNoramlized()) * scaleSpeed);
	}
	else
	{
		value = cameraSpringArm->TargetArmLength + (timelineCurve->GetFloatValue(GetSpringArmLenNoramlized()) * scaleSpeed);
	}
	//currentSpringArmLength += v;
	currentSpringArmLength = FMath::Clamp<float>(value, minSpringArmLength, maxSpringArmLength);
}

void AMyPawn::Acceleration()
{
	isAcceleration = true;
	speedIsUp = true;
}

void AMyPawn::EndAcceleration()
{
	isAcceleration = true;
	speedIsUp = false;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	playerController = UGameplayStatics::GetPlayerController(this, 0);

	FOnTimelineFloatStatic timelineCallBack;
	timelineCallBack.BindUFunction(this, TEXT("FocusMov"));
	timeline.AddInterpFloat(timelineCurve, timelineCallBack);
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//相机聚焦时间轴
	timeline.TickTimeline(DeltaTime);
	//相机缩放
	if (springArmCurve) 
	{
		zoomRate = springArmCurve->GetFloatValue(GetSpringArmLenNoramlized());
	}
	cameraSpringArm->TargetArmLength = FMath::FInterpTo(cameraSpringArm->TargetArmLength, currentSpringArmLength, GetWorld()->DeltaTimeSeconds, zoomRate);
	//是否加速
	if (isAcceleration) 
	{
		SpeedUp(DeltaTime);
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	//action event
	PlayerInputComponent->BindAction("CanMove", IE_Pressed, this, &AMyPawn::CanMove);//Bind mouse pressed event
	PlayerInputComponent->BindAction("CanMove", IE_Released, this, &AMyPawn::CanNotMove);//Bind mouse released event

	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &AMyPawn::Acceleration);//bind shift speed up
	PlayerInputComponent->BindAction("Shift", IE_Released, this, &AMyPawn::EndAcceleration);//bind shift speed up finish

	//axis event
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForWorld", this, &AMyPawn::MoveForWorld);
	PlayerInputComponent->BindAxis("MoveUp", this, &AMyPawn::MoveUp);
	PlayerInputComponent->BindAxis("Zoom", this, &AMyPawn::Zoom);
	PlayerInputComponent->BindAxis("TurnRight", this, &AMyPawn::TurnRight);
	PlayerInputComponent->BindAxis("TurnUp", this, &AMyPawn::TurnUp);
	
}

