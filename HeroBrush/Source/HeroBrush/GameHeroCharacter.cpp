// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
AGameHeroCharacter::AGameHeroCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	CameraThird = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroThirdCamera"));
	CameraThird->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraThird->SetRelativeLocation(FVector(20.0f, 100.0f, 100.0f));
	CameraThird->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
}

void AGameHeroCharacter::ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange) {
	if (IsLong) {

	}
	else {
		ChangeOnceEnergy(EnergyRange);
	}
}
void AGameHeroCharacter::ChangeOnceEnergy(float EnergyRange) {
	if (CurEnergy + EnergyRange < TotalEnergy && CurEnergy + EnergyRange > 0)
		CurEnergy += EnergyRange;
	else if (CurEnergy + EnergyRange >= TotalEnergy)
		CurEnergy = TotalEnergy;
	else if (CurEnergy + EnergyRange <= 0)
		CurEnergy = 0.0f;
}

bool AGameHeroCharacter::GetDeathStatus()
{
	return isDead;
}

