// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MYGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

	// 增加分数
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Points);

	//加上UFUNCTION会报紫
	int32 GetScore() const;

protected:
	// 玩家当前分数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score", ReplicatedUsing = OnRep_PlayerScore)
	int32 PlayerScore;

	UFUNCTION()
	void OnRep_PlayerScore();

	// 重写以支持分数同步
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
