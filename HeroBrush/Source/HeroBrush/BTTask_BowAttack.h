// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameFramework/Character.h"
#include "BTTask_BowAttack.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API UBTTask_BowAttack : public UBTTaskNode
{
	GENERATED_BODY()

 protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	
	UPROPERTY(EditAnywhere, Category = "AI_Attack")
		UAnimMontage* AttackAnim1;

	
};

