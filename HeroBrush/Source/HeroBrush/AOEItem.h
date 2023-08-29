// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOEItem.generated.h"

class USphereComponent;
UCLASS()
class HEROBRUSH_API AAOEItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* EffectComp;
	
public:	
	// Sets default values for this actor's properties
	AAOEItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		bool DamageFrom = true; // true来自于角色，false来自于敌人
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		int AOEInfo = 0; // 不同类型的伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float AOEDamage = -20.f;// 伤害的大小

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
