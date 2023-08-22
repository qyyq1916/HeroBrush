// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

 void  AHeroAIController::BeginPlay()
{
	
	 Super::BeginPlay();
	
	
	if (ensure(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (MyPawn)
	{
		
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
}


