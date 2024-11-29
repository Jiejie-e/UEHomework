// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetCube.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "../MyGameProjectile.h"
#include "../MyGameGameMode.h"
#include "../MyGameCharacter.h"
#include "GameFramework/GameMode.h"
#include "MyPlayerState.h"
#include "MyGameStateBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATargetCube::ATargetCube()
{
	// 启用网络复制
	bReplicates = true;
	bAlwaysRelevant = true; // 确保在所有客户端同步

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	CubeMesh->OnComponentHit.AddDynamic(this, &ATargetCube::OnHit);

	ScoreValue = 10;

	ScaleFactor = 0.5;

	LifeBar = 2.0;

	CurrentScale = FVector(1.0f, 1.0f, 1.0f);

	bIsImportantTarget = false;
}

// Called when the game starts or when spawned
void ATargetCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetCube::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) {
		//UE_LOG(LogTemp, Log, TEXT("TargetCube.cpp OnHit():HasAuthority() is true"));
		if (OtherActor && OtherActor->IsA(AMyGameProjectile::StaticClass())) {
			APawn* InstigatorPawn = Cast<APawn>(OtherActor->GetInstigator());
			if (InstigatorPawn) {
				AMyGameCharacter* Character = Cast<AMyGameCharacter>(InstigatorPawn);
				if (Character) {
					int32 TempScore = ScoreValue;
					if (bIsImportantTarget) {
						TempScore *= 2;
					}
					AMyPlayerState* PlayerState = Character->GetPlayerState<AMyPlayerState>();
					if (PlayerState) {
						PlayerState->AddScore(TempScore);
					}
					else {
						UE_LOG(LogTemp, Log, TEXT("TargetCube.cpp OnHit(): PlayerState is null or false"));
					}
					AMyGameStateBase* GameState = GetWorld()->GetGameState<AMyGameStateBase>();
					if (GameState) {
						GameState->AddToTotalScore(TempScore);
					}
					else {
						UE_LOG(LogTemp, Log, TEXT("TargetCube.cpp OnHit(): GameState is null or false"));
					}
					if (ScaleFactor) {
						//FVector CurrentScale = CubeMesh->GetRelativeScale3D();
						//CubeMesh->SetRelativeScale3D(CurrentScale * ScaleFactor);
						CurrentScale = CubeMesh->GetRelativeScale3D() * ScaleFactor;
					}
					if (LifeBar) {
						LifeBar--;
						if (LifeBar <= 0) {
							Destroy();
						}
					}
				}
			}
		}
	}
}

void ATargetCube::OnRep_ScaleFactor()
{
	
		UE_LOG(LogTemp, Log, TEXT("TargetCube.cpp OnRep_ScaleFactor(): Entering success"));
		CubeMesh->SetRelativeScale3D(CurrentScale);
	
}



void ATargetCube::OnRep_LifeBar()
{
	
		UE_LOG(LogTemp, Log, TEXT("TargetCube.cpp OnRep_LifeBar(): LifeBar is %f"), LifeBar);
		/*
		if (LifeBar <= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("TargetCube.cpp OnRep_LifeBar(): Will Destroy()"));
			//Destroy();
		}
	*/
}

void ATargetCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 确保 TotalScore 在网络上同步
	DOREPLIFETIME(ATargetCube, CurrentScale);
	DOREPLIFETIME(ATargetCube, LifeBar);
	DOREPLIFETIME(ATargetCube, bIsImportantTarget);
}

void ATargetCube::SetImportantTargetEffect()
{
	if (CubeMesh)
	{
		// 获取当前材质
		UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(
			nullptr,
			TEXT("/Game/StarterContent/Materials/M_Brick_Clay_Old.M_Brick_Clay_Old")
		);

		// 如果动态材质实例创建成功
		if (NewMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetCube.cpp SetImportantTargetEffect():Successfully loaded new material."));

			CubeMesh->SetMaterial(0, NewMaterial);

			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetCube.cpp SetImportantTargetEffect():Failed to load material."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetCube.cpp SetImportantTargetEffect(): CubeMesh is null"));
	}
}
void ATargetCube::OnRep_IsImportantTarget()
{
	if (bIsImportantTarget) {
		SetImportantTargetEffect();
	}
	
}
