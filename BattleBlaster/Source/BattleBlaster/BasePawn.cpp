// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);

	FRotator InterpolatedRotation = FMath::RInterpTo(
		TurretMesh->GetComponentRotation(),
		LookAtRotation,
		GetWorld()->GetDeltaSeconds(),
		10.0f
	);

	TurretMesh->SetWorldRotation(InterpolatedRotation);

}


void ABasePawn::Fire()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	

	if (Projectile) {
		Projectile->SetOwner(this);
		AActor* ProjectileOwner = Projectile->GetOwner();
		if (ProjectileOwner) {
			UE_LOG(LogTemp, Display, TEXT("Owner of this bullet: %s"), *ProjectileOwner->GetActorNameOrLabel());
		}
	}
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (ExplodeSound) {

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplodeSound, GetActorLocation());

	}


	if (DeathCameraShakeClass) {

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController) {
			PlayerController->ClientStartCameraShake(DeathCameraShakeClass);
		}
	}
}

