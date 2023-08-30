// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FuckerCutter.h"
#include "Enemy.h"
#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "CutDeckWindow.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API UCutDeckWindow : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()
	
public:
	AFuckerCutter* FuckerCutter = nullptr;
	AEnemy* ThisEnemy = nullptr;
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
