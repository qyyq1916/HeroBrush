// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeroBrushCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AHeroBrushCharacter

AHeroBrushCharacter::AHeroBrushCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	MyArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character MyArms"));
	MyArms->SetOnlyOwnerSee(true);
	MyArms->SetupAttachment(CameraThird);
	MyArms->bCastDynamicShadow = false;
	MyArms->CastShadow = false;
	// MyArms->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	MyArms->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Create a follow camera
	CameraThird = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroThirdCamera"));
	CameraThird->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraThird->SetRelativeLocation(FVector(0.0f, 30.0f, 30.0f));
	CameraThird->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	CameraShoot = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroShootCamera"));
	CameraShoot->SetupAttachment(CameraBoom);
	CameraShoot->SetRelativeLocation(FVector(0.0f, 70.0f, 70.0f));


}

void AHeroBrushCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		

		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHeroBrushCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroBrushCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHeroBrushCharacter::Look);

		//changeView
		PlayerInputComponent->BindAction("ChangeView", IE_Pressed, this, &AHeroBrushCharacter::ChangeView);

	}

}

void AHeroBrushCharacter::ChangeView()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (CameraThird->IsActive()) {
		CameraBoom->TargetArmLength = 200.0f;
		CameraShoot->SetActive(true);

		CameraThird->SetActive(false);
		
	}
	else if (CameraShoot->IsActive()) {
		CameraBoom->TargetArmLength = 400.0f;
		CameraThird->SetActive(true);

		CameraShoot->SetActive(false);
	}
	else {
		CameraBoom->TargetArmLength = 400.0f;
		CameraThird->SetActive(true);	
		CameraShoot->SetActive(false);
	}
}

void AHeroBrushCharacter::ChangeHealth(bool IsLong, int TimePeriod, float HealthRange) {

	if (IsLong) {
		
	}
	else {
		ChangeOnceHealth(HealthRange);
	}

}
void AHeroBrushCharacter::ChangeOnceHealth(float HealthRange) {
	if (CurHealth + HealthRange < TotalHealth && CurHealth + HealthRange > 0)
		CurHealth += HealthRange;
	else if (CurHealth + HealthRange >= TotalHealth)
		CurHealth = TotalHealth;
	else if (CurHealth + HealthRange <= 0)
		CurHealth = 0.0f;
}




void AHeroBrushCharacter::ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange) {
	if (IsLong) {
		
	}
	else {
		ChangeOnceEnergy(EnergyRange);
	}
}
void AHeroBrushCharacter::ChangeOnceEnergy(float EnergyRange) {
	if (CurEnergy + EnergyRange < TotalEnergy && CurEnergy + EnergyRange > 0)
		CurEnergy += EnergyRange;
	else if (CurEnergy + EnergyRange >= TotalEnergy)
		CurEnergy = TotalEnergy;
	else if (CurEnergy + EnergyRange <= 0)
		CurEnergy = 0.0f;
}

//FRotator AHeroBrushCharacter::GetAimRotation() {

//}