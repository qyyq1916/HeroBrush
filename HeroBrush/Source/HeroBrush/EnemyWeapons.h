// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroCharacter.h"
#include "EnemyWeapons.generated.h"

UCLASS()
class HEROBRUSH_API AEnemyWeapons : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyWeapons();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "knifeMesh")
		class USkeletalMeshComponent* KnifeMeshComponent;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "knifeMesh")
		USkeletalMesh* KnifeMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TArray")
		TArray<FName> EnemyKnifePointNames_Array;//需要获取位置的插槽的名字的容器
	TArray<FVector> EnemyKnifePointLocation_Array;//刀上插槽位置的容器

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TArray")
		TArray<TEnumAsByte<EObjectTypeQuery>> EnemyObjectType;
	TArray<AActor*> EnemyIgnoreActors_Array;
	TArray<FHitResult> EnemyHitResult;
	void  MontageWindowBegin();
	void  MontageWindowEnd();
	void  MontageWindowBegin_Delay();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 void GetTrancePointsLocation();
	 UPROPERTY(BlueprintReadOnly, Category = "Man")
		 TArray<AHeroCharacter*>HittingCharacter_Array;
	 AHeroCharacter* HittingCharacter = nullptr;
	 FTimerHandle Attacktest;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
