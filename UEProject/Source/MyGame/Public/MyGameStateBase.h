// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ScriptDelegates.h"
#include "Delegates/DelegateCombinations.h" 
#include "MyGameStateBase.generated.h"
/**
 * 
 */
UCLASS()
class MYGAME_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMyGameStateBase();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddToTotalScore(int32 Points);
	// 获取总分数
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetTotalScore() const;

	UFUNCTION(BlueprintCallable, Category = "Targets")
	void InitializeImportantTargets(int32 NumImportantTargets);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// 当前总分数
	//UPROPERTY(Replicated) 告诉引擎，这个变量支持复制。
	//有时，我们希望在客户端接收到更新后执行特定逻辑。可以为同步变量设置一个 OnRep 函数：
	/*
	UFUNCTION()
	void OnRep_TotalScore()
	{
		// 客户端接收到 TotalScore 更新后执行的逻辑
		UE_LOG(LogTemp, Log, TEXT("TotalScore updated to: %d"), TotalScore);
	}
*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score", ReplicatedUsing = OnRep_TotalScore)
	int32 TotalScore;

	UFUNCTION()
	void OnRep_TotalScore();

	// 重写以支持分数同步
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// 游戏时限
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float TimeLimit;

	// 剩余时间
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	float RemainingTime;

	UFUNCTION()
	void StartGameTimer();
protected:
	// 计时器处理
	FTimerHandle GameTimerHandle;  // 用于控制定时器

	// 每秒更新剩余时间
	void UpdateRemainingTime();

	// 游戏结束
	void EndGame();

	// 打印最终分数
	void PrintFinalScores();

};
