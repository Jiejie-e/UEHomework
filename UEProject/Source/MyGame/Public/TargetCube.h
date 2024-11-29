// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetCube.generated.h"

UCLASS()
class MYGAME_API ATargetCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ScaleFactor)
	FVector CurrentScale;

	UFUNCTION()
	void OnRep_ScaleFactor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 ScoreValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScaleFactor")
	float ScaleFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScaleFactor", ReplicatedUsing = OnRep_LifeBar)
	float LifeBar;

	UFUNCTION()
	void OnRep_LifeBar();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target", ReplicatedUsing=OnRep_IsImportantTarget)
	bool bIsImportantTarget;

	UFUNCTION()
	void SetImportantTargetEffect();
	UFUNCTION()
	void OnRep_IsImportantTarget();
};
