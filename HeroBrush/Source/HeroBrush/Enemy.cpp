// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "BaseWeaponBullets.h"
#include "AOEItem.h"
// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	 PrimaryActorTick.bCanEverTick = true;
	 StatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
	 StatusWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//CheckTouchActor(OtherActor);
	if (CurHealth <= 0) {
		Destroy();
	}
}

void AEnemy::SetTarget(AActor* NewTarget)
{
	if (NewTarget)
	{
		Target = NewTarget;
	}
}

void AEnemy::RemoteAttack()
{
	FVector BowLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Direction = Target->GetActorLocation() - BowLocation;

	FRotator BowRotation = Direction.Rotation();

	FActorSpawnParameters params;

	params.Instigator = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ensure(ProjectileClass);

	GetWorld()->SpawnActor<AActor>(ProjectileClass, BowLocation, BowRotation);
}

void AEnemy::AoeAttack()
{
	FVector BowLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Direction = Target->GetActorLocation() - BowLocation;

	FRotator BowRotation = Direction.Rotation();

	FActorSpawnParameters params;

	params.Instigator = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ensure(AOEItem);
	GetWorld()->SpawnActor<AActor>(AOEItem, BowLocation, BowRotation);
}