// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBulletsBase.h"

AEnemyBulletsBase::AEnemyBulletsBase()
{

	DamageFrom = false; // true来自于角色，false来自于敌人
	
	BulletDamage = 5.0f;//基础子弹伤害
}