// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BowAttack.h"
#include "AIController.h"
#include "HeroCharacter.h"
#include "Boss.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_BowAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if (ensure(MyController))
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetPawn());
		
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		MyPawn->SetTarget(TargetActor);

		if (MyPawn->GetCurrentMontage() != AttackAnim1)
		{
			MyPawn->PlayAnimMontage(AttackAnim1);
		}

		
		//return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}