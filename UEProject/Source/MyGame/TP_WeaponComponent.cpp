// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "MyGameCharacter.h"
#include "MyGameProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	
	AActor* Actor = GetOwner();
	if (Actor)
	{
		UE_LOG(LogTemp, Log, TEXT("TP_WeaponComponent.cpp UTP_WeaponComponent(): %s"), *Actor->GetName());
		Actor->SetReplicates(true);         // 启用复制
		Actor->SetReplicateMovement(true);  // 启用复制移动
	}
	
	//这个不知道能不能删除
	SetIsReplicatedByDefault(true);

	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("TP_WeaponComponent.cpp Fire():Entering in Fire()"));
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			if (GetOwner()->HasAuthority()) {
			//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				ActorSpawnParams.Instigator = Character;
				ActorSpawnParams.Owner = Character;
				World->SpawnActor<AMyGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				UE_LOG(LogTemp, Log, TEXT("TP_WeaponComponent.cpp Fire():GetOwner()->HasAuthority() is true"));
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("TP_WeaponComponent.cpp Fire():GetOwner()->HasAuthority() is false"));
				ServerFire(SpawnLocation, SpawnRotation);
			}
			
	
			// Spawn the projectile at the muzzle
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::ServerFire_Implementation(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (ProjectileClass != nullptr && GetWorld() != nullptr) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = Character;
		ActorSpawnParams.Owner = Character;
		GetWorld()->SpawnActor<AMyGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}

bool UTP_WeaponComponent::ServerFire_Validate(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	return true;
}

bool UTP_WeaponComponent::AttachWeapon(AMyGameCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	AActor* WeaponActor = GetOwner(); // 确保当前组件所在的 Actor
	if (WeaponActor)
	{
		WeaponActor->SetOwner(Character);
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("TP_WeaponComponent.cpp AttachWeapon():AttachWeapon()is successed"));
	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}