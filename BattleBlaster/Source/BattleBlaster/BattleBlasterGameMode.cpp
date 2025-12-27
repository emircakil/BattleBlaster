// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BattleBlasterGameInstance.h"
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

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController) {
		
	 ScreenMessageWidget = 
		CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
	 if (ScreenMessageWidget) {
		 ScreenMessageWidget->AddToPlayerScreen();
		
	 }
	}


	CountdownSeconds = CountdownDelay;

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountDownTimerTimeout,
			1.0f, true);
		
}

void ABattleBlasterGameMode::OnCountDownTimerTimeout()
{
	CountdownSeconds -= 1;

	if (CountdownSeconds > 0) {

	
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));

	} 
	else if (CountdownSeconds == 0) {
		ScreenMessageWidget->SetMessageText("GO!!");
		Tank->SetPlayerEnabled(true);
	}
	else {
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}



void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank)
	{ // Tank just died
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else {
		// Tower just died
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) {
			DeadTower->HandleDestruction();

			if (TowerCount > 0) {
				TowerCount--;
				if (!TowerCount) {
					IsVictory = true;
					IsGameOver = true;
					//UE_LOG(LogTemp, Display, TEXT("VICTORY!!!!"));
				}
			}		
		}
	}
	if (IsGameOver) {
		FString GameOverString = IsVictory ? "Victory!!" : "Defeat!";
		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);


		FTimerHandle FireTimerHandle;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, true);

	}

}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	

	if (UGameInstance* GameInstance = GetGameInstance()) {
	
		if (UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance)) {
		
			if (IsVictory) {
				// Load next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else {
				//Repeat current level
				BattleBlasterGameInstance->RestartCurrentLevel();

			}
		}
	}
}


