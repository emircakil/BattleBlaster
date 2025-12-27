// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplodeSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathParticles;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	void RotateTurret(FVector LookAtTarget);
	
	void Fire();

	void HandleDestruction();

};
