// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class HEROBRUSH_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	//SphereComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
		class USphereComponent* SpereComp;

	//Two Overlap Events
	//OnOverlapBegin and OnOverlapEnd
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
		UStaticMeshComponent* MeshComp;

	//ParticleComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Particle")
		class UParticleSystemComponent* ParticleComp;

	//ParticleOverlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
		UParticleSystem* ParticleOverlap;

	//SoundOverlap(To Be Realized)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue* SoundOverlap;

	//Rotate(To Be Realized)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
		bool Rotate;

	//RotationSpeed(To Be Realized)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
		float RotationSpeed;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
