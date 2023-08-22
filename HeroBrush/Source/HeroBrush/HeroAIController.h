// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroAIController.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API AHeroAIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;
	

	virtual void BeginPlay() override;
};
