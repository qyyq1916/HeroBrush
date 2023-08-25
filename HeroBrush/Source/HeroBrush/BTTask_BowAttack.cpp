// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BowAttack.h"
#include "AIController.h"
#include "HeroCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_BowAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector BowLocation = MyPawn->GetMesh()->GetSocketLocation("hand_l");
		
		FVector Direction = TargetActor->GetActorLocation() - BowLocation;

		FRotator BowRotation = Direction.Rotation();

		FActorSpawnParameters params;

		params.Instigator = MyPawn;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ensure(ProjectileClass);
		
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, BowLocation, BowRotation);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}