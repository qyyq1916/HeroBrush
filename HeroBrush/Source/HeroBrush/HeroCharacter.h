// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HeroCharacter.generated.h"

UCLASS()
class HEROBRUSH_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()
		/** Camera boom positioning the camera behind the character */
public:
	

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	// health system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurHealth = 100.0f;
	UFUNCTION(BlueprintCallable)
		void ChangeHealth(bool IsLong, int TimePeriod, float HealthRange);  // 检测如果是长期加血，那么使用的是下面的状态来决定是否停止加血。
	
	void ChangeOnceHealth(float HealthRange); // 直接加这么多。
		
public:
	// Sets default values for this character's properties
	AHeroCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector AimLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector FromLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator AimRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDead = false;

	UFUNCTION()
		FRotator GetAimRotation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, Category = "Hurt")
		UAnimMontage* CharacterHurtAnim;
	int BaTiNumber=8;
public:	
	void SetIsEnemyTrue();
	bool IsEnemy = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FTimerDelegate HealthDelegate;
	FTimerHandle HealthTimer;
	UFUNCTION(BlueprintCallable)
		void ClearFunction();
};
