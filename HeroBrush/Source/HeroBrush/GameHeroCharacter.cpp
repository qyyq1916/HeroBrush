// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayController.h"

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

void AGameHeroCharacter::CheckForInteractables() {
	// 射线拾取
	FHitResult HitResult; // 存储碰撞的结果的变量

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector MuzzleOffset;
	MuzzleOffset.Set(100.0f, 20.0f, 0.0f);

	FVector StartTrace = CameraLocation; // 射线起始点
	FVector EndTrace = (FTransform(CameraRotation).TransformVector(MuzzleOffset)) * 2.f + StartTrace; // 射线终止点。

	FCollisionQueryParams QueryParams; // 储存了碰撞相关的信息
	QueryParams.AddIgnoredActor(this); // 将我们角色自身忽略掉，减少性能开销

	AGameplayController* controller = Cast<AGameplayController>(GetController()); // 玩家控制器

	/*TArray<AActor*> IgnoreActors;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);
	if (bIsHit)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), HitResult.GetActor()->GetName());
	}*/

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams) && controller) {
		//检查我们点击的项目是否是一个可交互的项目
		if (AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor())) {
			controller->CurrentInteractable = Interactable;
			return;
		}
	}

	//如果我们没有击中任何东西，或者我们击中的东西不是一个可交互的，设置currentinteractable为nullptr
	controller->CurrentInteractable = nullptr;

}

