// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_NearRange.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API UBTService_NearRange : public UBTService
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector NearRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
