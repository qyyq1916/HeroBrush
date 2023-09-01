// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AHeroCharacter::AHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//		
	//	}
	//}
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
}

void AHeroCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHeroCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

FRotator AHeroCharacter::GetAimRotation() {
	AimRotation = (AimLocation - FromLocation).ToOrientationRotator();
	return AimRotation;
}

void AHeroCharacter::ClearFunction()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	HealthDelegate.Unbind();
}

void AHeroCharacter::ChangeHealth(bool IsLong, int TimePeriod, float HealthRange) {

	if (IsLong) {
		HealthDelegate.BindUObject(this, &AHeroCharacter::ChangeOnceHealth, HealthRange); // 绑定带参数HealthRange的这个函数和HealthDelegate.
		GetWorld()->GetTimerManager().SetTimer(HealthTimer, HealthDelegate, TimePeriod, true, -1); // 循环调用每TimePeriod使用一次，一直循环
		// 什么时候离开，应该在离开这个邻域之后停止掉血，这个什么时候停止写在我们的子类里面,写成下面这样，就是在当前状态只有一种持续的方式，
		// 如果需要重叠，需要多加一个代理
		/*
		GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
		HealthDelegate.Unbind();
		*/
	}
	else {
		ChangeOnceHealth(HealthRange);
	}

}
void AHeroCharacter::ChangeOnceHealth(float HealthRange) {
	if (HealthRange < 0) {
		if (IsEnemy) {
			PlayAnimMontage(CharacterHurtAnim);
		}
		else {
			BaTiNumber--;
			if (BaTiNumber == 0) {
				BaTiNumber = 8;
				PlayAnimMontage(CharacterHurtAnim);
			}
		}
		
	}
	if (CurHealth + HealthRange < TotalHealth && CurHealth + HealthRange > 0)
		CurHealth += HealthRange;
	else if (CurHealth + HealthRange >= TotalHealth)
		CurHealth = TotalHealth;
	else if (CurHealth + HealthRange <= 0)
		CurHealth = 0.0f;

	if (CurHealth <= 0) {
		isDead = true;
	}
	else if (CurHealth > 0) {
		isDead = false;
	}

}
void AHeroCharacter::SetIsEnemyTrue() {
	IsEnemy = true;
}