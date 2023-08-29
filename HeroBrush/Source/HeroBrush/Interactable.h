// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class HEROBRUSH_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	UPROPERTY(EditDefaultsOnly)
		FString Name;

	UPROPERTY(EditDefaultsOnly)
		FString _Action;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		FString GetUseText() const { return FString::Printf(TEXT("%s : Press F to %s"), *Name, *_Action); }

	UFUNCTION(BlueprintImplementableEvent)
		void Interact(APlayerController* controller);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
