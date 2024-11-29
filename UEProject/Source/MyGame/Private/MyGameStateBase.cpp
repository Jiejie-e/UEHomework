// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "TargetCube.h"
#include "TimerManager.h"
#include "MyPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


AMyGameStateBase::AMyGameStateBase()
{
	bReplicates = true;
	TotalScore = 0;
	TimeLimit = 60.0f;
	RemainingTime = TimeLimit;

}

void AMyGameStateBase::AddToTotalScore(int32 Points)
{
	TotalScore += Points;
	UE_LOG(LogTemp, Log, TEXT("MyGameGameMode.cpp AddToTotalScore():TotalScore is %d"), TotalScore);
}

int32 AMyGameStateBase::GetTotalScore() const
{
	return TotalScore;
}

void AMyGameStateBase::InitializeImportantTargets(int32 NumImportantTargets)
{
	if (!HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("GameStateBase.cpp: Only the server can initialize important targets."));
		return;
	}
	TArray<AActor*> TargetCubes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetCube::StaticClass(), TargetCubes);

	int32 TotalTargets = TargetCubes.Num();
	if (TotalTargets == 0 || NumImportantTargets <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("GameStateBase.cpp: No targets found or invalid number of important targets."));
		return;
	}

	NumImportantTargets = FMath::Min(NumImportantTargets, TotalTargets);

	// 随机选择重要目标
	TArray<int32> SelectedIndices;
	while (SelectedIndices.Num() < NumImportantTargets) {
		int32 RandomIndex = FMath::RandRange(0, TotalTargets - 1);
		if (!SelectedIndices.Contains(RandomIndex)) {
			SelectedIndices.Add(RandomIndex);

			ATargetCube* TargetCube = Cast<ATargetCube>(TargetCubes[RandomIndex]);
			if (TargetCube) {
				TargetCube->bIsImportantTarget = true;
				UE_LOG(LogTemp, Log, TEXT("GameStateBase.cpp: Selected TargetCube as important target: %s"), *TargetCube->GetName());

				// 设置特殊颜色或特效
			}
		}
	}

}
void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) // 确保仅服务器执行
	{
		UE_LOG(LogTemp, Log, TEXT("GameStateBase.cpp: Initializing important targets on server."));

		// 假设我们希望随机设置 5 个重要目标
		int32 NumImportantTargets = 5;
		InitializeImportantTargets(NumImportantTargets);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("GameStateBase.cpp BeginPlay(): HasAuthority() is false"));
	}

	StartGameTimer();
}

void AMyGameStateBase::OnRep_TotalScore()
{
	UE_LOG(LogTemp, Log, TEXT("MyGameGameMode.cpp TotalScore is %d"), TotalScore);
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 确保 TotalScore 在网络上同步
	DOREPLIFETIME(AMyGameStateBase, TotalScore);

}

void AMyGameStateBase::StartGameTimer()
{
	if (HasAuthority())
	{
		// 设置计时器，每秒更新剩余时间
		GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AMyGameStateBase::UpdateRemainingTime, 1.0f, true);

		UE_LOG(LogTemp, Log, TEXT("GameState: Game started. Time limit: %f seconds"), TimeLimit);
	}
}

void AMyGameStateBase::UpdateRemainingTime()
{
	if (RemainingTime > 0)
	{
		RemainingTime--;
	}
	else
	{
		// 时间到，结束游戏
		EndGame();
	}
}

void AMyGameStateBase::EndGame()
{
	GetWorldTimerManager().ClearTimer(GameTimerHandle);

	UE_LOG(LogTemp, Log, TEXT("AMyGameStateBase.cpp EndGame(): Time's up. Calculating scores..."));


	// 输出每个玩家的得分和总分
	PrintFinalScores();
}

void AMyGameStateBase::PrintFinalScores()
{
	AMyGameStateBase* GameState = GetWorld()->GetGameState<AMyGameStateBase>();
	if (GameState) {
		UE_LOG(LogTemp, Log, TEXT("AMyGameStateBase.cpp PrintFinalScores(): Total Score is %d"), GameState->GetTotalScore());
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("AMyGameStateBase.cpp PrintFinalScores(): GameState is null or false"));
	}
	for (APlayerState* PlayerState : PlayerArray) {
		AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(PlayerState);
		if (MyPlayerState) {
			UE_LOG(LogTemp, Log, TEXT("AMyGameStateBase.cpp PrintFinalScores(): Player%s Score is %d"),*MyPlayerState->GetPlayerName(), MyPlayerState->GetScore());
		}
	}
}

