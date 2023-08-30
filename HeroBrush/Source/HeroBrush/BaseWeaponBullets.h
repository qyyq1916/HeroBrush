// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponBullets.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class HEROBRUSH_API ABaseWeaponBullets : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;
	/*Effect Component*/
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* EffectComp; 

public:
	// Sets default values for this actor's properties
	ABaseWeaponBullets();

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float BulletDamage = -10.f;//基础子弹伤害
	UFUNCTION()
		void ChangeBulletDamage(float change);
};