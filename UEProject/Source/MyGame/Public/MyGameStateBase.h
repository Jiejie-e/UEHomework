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
	// ��ȡ�ܷ���
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetTotalScore() const;

	UFUNCTION(BlueprintCallable, Category = "Targets")
	void InitializeImportantTargets(int32 NumImportantTargets);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// ��ǰ�ܷ���
	//UPROPERTY(Replicated) �������棬�������֧�ָ��ơ�
	//��ʱ������ϣ���ڿͻ��˽��յ����º�ִ���ض��߼�������Ϊͬ����������һ�� OnRep ������
	/*
	UFUNCTION()
	void OnRep_TotalScore()
	{
		// �ͻ��˽��յ� TotalScore ���º�ִ�е��߼�
		UE_LOG(LogTemp, Log, TEXT("TotalScore updated to: %d"), TotalScore);
	}
*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score", ReplicatedUsing = OnRep_TotalScore)
	int32 TotalScore;

	UFUNCTION()
	void OnRep_TotalScore();

	// ��д��֧�ַ���ͬ��
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// ��Ϸʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float TimeLimit;

	// ʣ��ʱ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	float RemainingTime;

	UFUNCTION()
	void StartGameTimer();
protected:
	// ��ʱ������
	FTimerHandle GameTimerHandle;  // ���ڿ��ƶ�ʱ��

	// ÿ�����ʣ��ʱ��
	void UpdateRemainingTime();

	// ��Ϸ����
	void EndGame();

	// ��ӡ���շ���
	void PrintFinalScores();

};
