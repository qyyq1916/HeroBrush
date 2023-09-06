// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemy.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnEnemy::ASpawnEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnEnemyRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnEnemyBox"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASpawnEnemy::BeginPlay()
{
	Super::BeginPlay();
	SpawnSomeEnemy();
	SetLifeSpan(SpawnTime);
}

// Called every frame
void ASpawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnEnemy::SpawnSomeEnemy()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnEnemy::SpawnOneEnemy, SpawnPeriod, true, SpawnAppearTime);
}
void ASpawnEnemy::SpawnOneEnemy()
{
	FVector BornLocation = GetActorLocation();
	FRotator BornRotation = GetActorRotation();

	const FTransform SpawnTM = FTransform(BornRotation, BornLocation);
	
	
	GetWorld()->SpawnActor<AActor>(New_Enemy, SpawnTM);
}
