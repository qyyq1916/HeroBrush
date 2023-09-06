// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroBrushCharacter.h"
#include "GameplayController.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API AGameplayController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class AInteractable* CurrentInteractable;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FInventoryItem> Inventory;

	UFUNCTION(BlueprintCallable, Category = "Utils")
		void AddItemToInventoryByID(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Utils")
		void MinusItemToInventoryByID(FName ID, int& OriginQuantity, int& AfterQuantity);

	UFUNCTION(BlueprintImplementableEvent)
		void ReloadInventory();

	UFUNCTION(BlueprintCallable, Category = "Utils")
		void CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, AGameplayController* controller);

protected:
	void Interact();
	virtual void SetupInputComponent() override;
};
