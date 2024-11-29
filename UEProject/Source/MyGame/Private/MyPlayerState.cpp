// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{
	PlayerScore = 0;
}

void AMyPlayerState::AddScore(int32 Points)
{
	PlayerScore += Points;
}

int32 AMyPlayerState::GetScore() const
{
	return PlayerScore;
}

void AMyPlayerState::OnRep_PlayerScore()
{
	UE_LOG(LogTemp, Log, TEXT("MyPlayerState.cpp PlayerScore is %d"), PlayerScore);
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 确保 PlayerScore 在网络上同步
	DOREPLIFETIME(AMyPlayerState, PlayerScore);
}
