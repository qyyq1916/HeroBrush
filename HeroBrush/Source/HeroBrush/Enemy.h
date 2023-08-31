// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "EnemyWeapons.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor); // 暂时加入到这里测试，后续死亡动画需要在行为树中

	FTimerHandle DeathTimer;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* StatusWidgetComponent;
	// 受伤动画
	UPROPERTY(EditAnywhere, Category = "Enemy_Effect")
		UAnimMontage* HurtAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemyknife")
		AEnemyWeapons* EnemyKnife=nullptr;
	
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

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> AOEItem;

};
