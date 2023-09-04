// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "SpawnEnemy.generated.h"

UCLASS()
class HEROBRUSH_API ASpawnEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnEnemy();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemy> New_Enemy;
	UFUNCTION()
		void SpawnSomeEnemy();
	UFUNCTION()
		void SpawnOneEnemy();
	FTimerHandle SpawnTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* SpawnVolume;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
