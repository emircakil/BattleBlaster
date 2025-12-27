// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"


// emo;'cemo;'pirtpirt

void UBattleBlasterGameInstance::ChangeLevel(int32 NextLevelIndex)
{
	if (NextLevelIndex > 0 && NextLevelIndex <= LastLevelIndex) {
		CurrentLevelIndex = NextLevelIndex;
		FString LevelNameString = FString::Printf(TEXT("Level_%d"), NextLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);

	}
}

void UBattleBlasterGameInstance::LoadNextLevel() {

	if (CurrentLevelIndex < LastLevelIndex) {
	
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else {
	
		RestartGame();
	}
}
void UBattleBlasterGameInstance::RestartCurrentLevel() {
	ChangeLevel(CurrentLevelIndex);
}
void UBattleBlasterGameInstance::RestartGame() {
	ChangeLevel(1);
}
