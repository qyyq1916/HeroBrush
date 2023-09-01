// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "GameHeroCharacter.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class HEROBRUSH_API AGameHeroCharacter : public AHeroCharacter
{
	GENERATED_BODY()
public:
	AGameHeroCharacter();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurEnergy = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
	class UCameraComponent* CameraThird;

	UFUNCTION(BlueprintCallable)
		bool GetDeathStatus();


	// function
	UFUNCTION(BlueprintCallable)
		void ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange);
	void ChangeOnceEnergy(float EnergyRange);
	
};
