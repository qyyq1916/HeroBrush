// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "EnemyWeapons.h"
#include "Camera/PlayerCameraManager.h"
#include "Enemy.generated.h"

/**
 * 
 */
class UWidgetComponent;
UCLASS()
class HEROBRUSH_API AEnemy : public AHeroCharacter
{
	GENERATED_BODY()
		
public:
	// Sets default values for this character's properties
	 AEnemy();
	 
	 bool IsFirstTime=true;
	 static int EnemyQuantity;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		 USoundWave* PirAttackSound;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		 USoundWave* PirAttackSound2;
	
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		 USoundWave* DeadSound;
public:
	 static FName WeaponSlot;
	 UUserWidget* EnemyClearWindows;
	 int EnemyTotal=0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor); 

	FTimerHandle DeathTimer;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* StatusWidgetComponent;
	// 受伤动画
	UPROPERTY(EditAnywhere, Category = "Enemy_Effect")
		UAnimMontage* HurtAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemyknife")
		AEnemyWeapons* EnemyKnife=nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "knifeMesh")
		USkeletalMesh* EnemyKnifeMesh;
	
	APlayerCameraManager* PlayerCameraManager;
	APlayerController* MyCharacterController;
public:
	void SetTarget(AActor* NewTarget);
	void RefreshHeadInfo();
protected:
	AActor* Target;

public:
	UFUNCTION(BlueprintCallable)
		void RemoteAttack();
	UFUNCTION(BlueprintCallable)
		void AoeAttack();
	UFUNCTION()
		void SetStatusRotation();
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> AOEItem;
	void SpawnKnife();
	bool IsNear=true;
};
int AEnemy::EnemyQuantity = 0;