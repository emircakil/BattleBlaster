// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn) {
	
		Tank = Cast<ATank>(PlayerPawn);
		if (Tank) {

		}
		else {
			UE_LOG(LogTemp, Display, TEXT("GameMode: PlayerPown couldn't converted to ATank"));
		}

	}
	
	int32 i = 0;
	while (i < TowerCount) {

		AActor* TowerActor = Towers[i];
		ATower* Tower = Cast<ATower>(TowerActor);

		if (Tower && Tank) {
			Tower->Tank = Tank;
			UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable"), *Tower->GetActorNameOrLabel());

		}

		i++;
	}
		
}
