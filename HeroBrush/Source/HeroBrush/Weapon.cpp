// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "HeroBrushCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(MeshComp);

	TextCue = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextCue"));
	TextCue->SetupAttachment(Mesh);
	TextCue->SetWorldSize(10.f);
	TextCue->SetTextRenderColor(FColor::Black);
	TextCue->SetVisibility(false);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		AHeroBrushCharacter* role = Cast<AHeroBrushCharacter>(OtherActor);
		if (role) {
			if (role->ActiveOverlapItem == nullptr) {
				role->ActiveOverlapItem = this;
			}
			TextCue->SetVisibility(true);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AHeroBrushCharacter* role = Cast<AHeroBrushCharacter>(OtherActor);
		if (role) {
			if (role->ActiveOverlapItem == this) {
				role->ActiveOverlapItem = nullptr;
			}
			TextCue->SetVisibility(false);
		}
	}

}

void AWeapon::Equip(AHeroBrushCharacter* owner) {
	if (owner) {
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetSimulatePhysics(false);
		AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WEAPON_R");
		Rotate = false;
		ParticleComp->SetActive(false);
		SpereComp->OnComponentBeginOverlap.RemoveDynamic(this, &AItem::OnOverlapBegin);
		SpereComp->OnComponentEndOverlap.RemoveDynamic(this, &AItem::OnOverlapEnd);
		TextCue->SetVisibility(false);
		//Set Sound here(We don't have any sound context.To Be Realized)
		if (SoundEquiped) {
			UGameplayStatics::PlaySound2D(this, SoundEquiped);
		}
		owner->SetWeapon(this);
	}

}