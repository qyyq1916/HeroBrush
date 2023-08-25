﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HeroCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HeroBrushCharacter.generated.h"


UCLASS(config=Game)
class AHeroBrushCharacter : public AHeroCharacter
{
	GENERATED_BODY()

public:
	//variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
		float SmoothBlendTime = 0.75f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurEnergy = 100.f;
	


	FString HealthStatus = "none"; // 分为特别的几个加血的状态。使用checktouchactor来维护这个状态决定如何加血。
	FString EnergyStatus = "none";

	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
		class UCameraComponent* CameraShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
		class UCameraComponent* CameraThird;
	
	// delegate
	FTimerDelegate HealthDelegate;
	FTimerDelegate EnergyDelegate;
	FTimerDelegate SpeedUpDelegate;

	//// action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ChangeViewAction;



	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* MyArms;
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetArms() const { return MyArms; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraThird; }
public:
	AHeroBrushCharacter();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	

	//function
	void ChangeView();
	void ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange);
	void ChangeOnceEnergy(float EnergyRange);
	void ChangeHealth(bool IsLong, int TimePeriod, float HealthRange);  // 检测如果是长期加血，那么使用的是下面的状态来决定是否停止加血。
	void ChangeOnceHealth(float HealthRange); // 直接加这么多。
	

	// inline function
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	///** Returns FollowCamera subobject **/
	//FORCEINLINE class UCameraComponent* GetFollowCameraFirst() const { return CameraFirst; }
	//FORCEINLINE class UCameraComponent* GetFollowCameraThird() const { return CameraThird; }


protected:

	//Basic Attack(No Weapon)
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim1;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim2;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim3;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_PrimaryAttack;

	// anim control
	int AttackAnimSeq = 0;
	FVector HandLocation;


	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		UAnimMontage* BurdenAnim; // Burden_Attack
	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		TSubclassOf<AActor> ProjectileClass_Burden;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_Burden_Attack;


	UPROPERTY(EditAnywhere, Category = "Flash_Attack")
		TSubclassOf<AActor> ProjectileClass_Flash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash_Attack")
		float FlashDistance = 600.0f;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_Flash_Attack;


	// 技能冷却
	UPROPERTY(EditAnywhere, Category = "Flash_Attack_TimePeriod")
		int Flash_Attack_Time = 180;
	bool isFlashActive = true;

	UPROPERTY(EditAnywhere, Category = "SpeedUp")
		float SetSpeedMax = 1000.0f;

	//// 加速事件代理,在有能量消耗的时候使用。
	//FTimerDelegate SpeedUpDelegate;

public:
	virtual void BeginPlay();
	//virtual void EndPlay(); 想要使用这个销毁定时器
	virtual void Tick(float DeltaTime) override;

protected:
	//Basic Attack Func(No Weapon)
	void Primary_Attack();

	void PrimaryAttack_TimeElapsed();

	void Burden_Attack();

	void Burden_Attack_TimeElapsed();

	void Flash_Attack();

	void Flash_TimeElapsed();
	
	void TurnOnSpeed();

	void TurnOffSpeed();
};

