// Fill out your copyright notice in the Description page of Project Settings.


#include "CutDeckWindow.h"
#include "FuckerCutter.h"

void UCutDeckWindow::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	FuckerCutter = Cast<AFuckerCutter>(BranchingPointPayload.SkelMeshComponent->GetOwner());
	if (FuckerCutter)
	{
		//BranchingPointPayload.SkelMeshComponent->GetOwner(
		FuckerCutter->MontageWindowBegin();
		FuckerCutter->IgnoreActors_Array.Emplace(FuckerCutter);//忽略玩家自己的碰撞
	}
	ThisEnemy= Cast<AEnemy>(BranchingPointPayload.SkelMeshComponent->GetOwner());
	if (ThisEnemy)
	{
		//BranchingPointPayload.SkelMeshComponent->GetOwner(
		ThisEnemy->EnemyKnife->MontageWindowBegin();
		ThisEnemy->EnemyKnife->EnemyIgnoreActors_Array.Emplace(ThisEnemy);//忽略玩家自己的碰撞
	}
}

void UCutDeckWindow::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (FuckerCutter)
	{
		FuckerCutter->MontageWindowEnd();
		FuckerCutter->IgnoreActors_Array.Empty();
		FuckerCutter = nullptr;
	}
	if (ThisEnemy)
	{
		ThisEnemy->EnemyKnife->MontageWindowEnd();
		ThisEnemy->EnemyKnife->EnemyIgnoreActors_Array.Empty();
		ThisEnemy = nullptr;
	}
	/*GEngine->AddOnScreenDebugMessage
	(
		-1,
		10, 			//	显示的时间/秒
		FColor::Blue, 	//	显示的颜色
		"fashe UE4"	//	显示的信息
	);*/
}