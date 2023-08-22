// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_Limit.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API UBTService_Limit : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector LimitKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
