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


	FVector newLocation;//����۽���λ��
	float newPitch = 0.0f;//���Ǵ�С
	float newYaw = 0.0f;//��ת�Ƕ�
	
	//�۽�
	void Focus();
	//�۽��ƶ�(ʱ���������к���)
	UFUNCTION()
	void FocusMov(float lerp);

	
private:
	class USpringArmComponent* cameraSpringArm;//�����
	class UCameraComponent* camera;//���
	APlayerController* playerController;//��ҿ�����
	//timeline
	FTimeline timeline;

	FVector currentLocation;//��ǰλ��
	FVector targetLocation;//Ŀ��λ��
	FRotator targetRotation;//Ŀ����ת����
	float currentPitch = 0.0f;//��ǰy�Ƕ�
	float currentYaw = 0.0f;//��ǰz�Ƕ�
	float add = -30.0f;
	float AccelerationFactor = 1;//����ϵ��
	float zoomRate = 0.5f;//Ĭ����������

	bool isTurn = false;//�Ƿ������ת
	bool isAcceleration = false;//�Ƿ����
	bool speedIsUp = false;//���ٻ��߻ָ�


	//��ͷ�Ƿ�����ƶ�
	void CanMove();
	void CanNotMove();
	//�����ƶ�
	void MoveRight(float v);
	//ǰ���ƶ�
	void MoveForWorld(float v);
	//�����ƶ�
	void MoveUp(float v);
	//������ת
	void TurnRight(float v);
	//������ת
	void TurnUp(float v);
	//����
	void Zoom(float v);
	//Shift����,ȡ������
	void Acceleration();
	void EndAcceleration();
	void SpeedUp(float DeltaTime);

	//��ȡ�������Normalized
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
