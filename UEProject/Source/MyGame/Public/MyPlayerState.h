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

	// ���ӷ���
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Points);

	//����UFUNCTION�ᱨ��
	int32 GetScore() const;

protected:
	// ��ҵ�ǰ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score", ReplicatedUsing = OnRep_PlayerScore)
	int32 PlayerScore;

	UFUNCTION()
	void OnRep_PlayerScore();

	// ��д��֧�ַ���ͬ��
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
