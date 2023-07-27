// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "MyPawn.generated.h"

UCLASS()
class DEMO_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn")
		float moveSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn")
		float rotationSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn")
		float scaleSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn", meta = (ClampMin = 1.0f))
		float maxAccelerationFactor = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|springArm")
		float maxSpringArmLength = 30000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|springArm")
		float minSpringArmLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|springArm")
		float currentSpringArmLength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|springArm")
		bool isLock = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|Rotation", meta = (ClampMin = -380.0f, ClampMax = 380.0f))
		float minAngle = -89.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|Rotation", meta = (ClampMin = -380.0f, ClampMax = 380.0f))
		float mmaxAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|Timeline", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))//focusAnimationRate
		float focusAnimationRate = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|Curve")
		class UCurveFloat* timelineCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Pawn|Curve")
		class UCurveFloat* springArmCurve;


	FVector newLocation;//相机聚焦新位置
	float newPitch = 0.0f;//仰角大小
	float newYaw = 0.0f;//旋转角度
	
	//聚焦
	void Focus();
	//聚焦移动(时间轴内运行函数)
	UFUNCTION()
	void FocusMov(float lerp);

	
private:
	class USpringArmComponent* cameraSpringArm;//相机臂
	class UCameraComponent* camera;//相机
	APlayerController* playerController;//玩家控制器
	//timeline
	FTimeline timeline;

	FVector currentLocation;//当前位置
	FVector targetLocation;//目标位置
	FRotator targetRotation;//目标旋转方向
	float currentPitch = 0.0f;//当前y角度
	float currentYaw = 0.0f;//当前z角度
	float add = -30.0f;
	float AccelerationFactor = 1;//加速系数
	float zoomRate = 0.5f;//默认缩放速率

	bool isTurn = false;//是否可以旋转
	bool isAcceleration = false;//是否加速
	bool speedIsUp = false;//加速或者恢复


	//镜头是否可以移动
	void CanMove();
	void CanNotMove();
	//左右移动
	void MoveRight(float v);
	//前后移动
	void MoveForWorld(float v);
	//上下移动
	void MoveUp(float v);
	//左右旋转
	void TurnRight(float v);
	//上下旋转
	void TurnUp(float v);
	//缩放
	void Zoom(float v);
	//Shift加速,取消加速
	void Acceleration();
	void EndAcceleration();
	void SpeedUp(float DeltaTime);

	//获取相机缩放Normalized
	virtual float GetSpringArmLenNoramlized();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
