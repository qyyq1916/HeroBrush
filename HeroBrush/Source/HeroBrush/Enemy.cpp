// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "BaseWeaponBullets.h"
#include "Components/ProgressBar.h"
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
	auto InfoWidegetClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/ThirdPerson/Widgets/EnemyStatus.EnemyStatus_C'"));
	StatusWidgetComponent->SetWidgetClass(InfoWidegetClass);
	StatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusWidgetComponent->SetPivot(FVector2D(1, 0.5));
	StatusWidgetComponent->SetDrawSize(FVector2D(120.0f, 10.0f));
	StatusWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 95.0f));
}
void AEnemy::RefreshHeadInfo()
{
	UUserWidget* InfoWidget = StatusWidgetComponent->GetUserWidgetObject();
	if (InfoWidget) {
		auto ProgressBar = Cast<UProgressBar>(InfoWidget->GetWidgetFromName(TEXT("AIHealth")));
		if (ProgressBar)
			ProgressBar->SetPercent(CurHealth / TotalHealth);
	}
}
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RefreshHeadInfo();
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