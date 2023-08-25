// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_NearRange.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_NearRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackBoardComp))
    {
        AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
        if (TargetActor)
        {
            AAIController* MyController = OwnerComp.GetAIOwner();

            if (ensure(MyController))
            {
                APawn* AIPawn = MyController->GetPawn();
                if (ensure(AIPawn))
                {
                    float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());


                    bool bWithinNear = Distance < 100.f;
                    BlackBoardComp->SetValueAsBool(NearRangeKey.SelectedKeyName, bWithinNear);
                }
            }
        }
    }

}


