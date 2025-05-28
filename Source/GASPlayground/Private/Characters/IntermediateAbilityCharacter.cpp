// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/IntermediateAbilityCharacter.h"

#include "AbilitySystemComponent.h"
#include "PlayerState/AbilityPlayerState.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Enums/AbilitySystemInputEnum.h"

// Sets default values
AIntermediateAbilityCharacter::AIntermediateAbilityCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AIntermediateAbilityCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIntermediateAbilityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// To set the avatar and owner actor on server
	AAbilityPlayerState* AbilityPlayerState = GetPlayerState<AAbilityPlayerState>();
	if (IsValid(AbilityPlayerState))
	{
		AbilityPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AbilityPlayerState, this);
	}
}

void AIntermediateAbilityCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// To set the avatar and owner actor on client
	AAbilityPlayerState* AbilityPlayerState = GetPlayerState<AAbilityPlayerState>();
	if (IsValid(AbilityPlayerState))
	{
		AbilityPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AbilityPlayerState, this);
	}
}

// Called every frame
void AIntermediateAbilityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AIntermediateAbilityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &AIntermediateAbilityCharacter::OnConfirmInput);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this, &AIntermediateAbilityCharacter::OnCancelInput);

		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &AIntermediateAbilityCharacter::OnPrimaryInput);
		EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Triggered, this, &AIntermediateAbilityCharacter::OnSecondaryInput);
	}

}

UAbilitySystemComponent* AIntermediateAbilityCharacter::GetAbilitySystemComponent() const
{
	AAbilityPlayerState* AbilityPlayerState = GetPlayerState<AAbilityPlayerState>();
	if (IsValid(AbilityPlayerState))
	{
		return AbilityPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

void AIntermediateAbilityCharacter::OnConfirmInput(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Confirm Input");
	GetAbilitySystemComponent()->LocalInputConfirm();
}

void AIntermediateAbilityCharacter::OnCancelInput(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Cancel Input");
	GetAbilitySystemComponent()->LocalInputCancel();
}

void AIntermediateAbilityCharacter::OnPrimaryInput(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInput::Primary));
}

void AIntermediateAbilityCharacter::OnSecondaryInput(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInput::Secondary));
}

void AIntermediateAbilityCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int InputID)
{
	if (const auto AbilitySystemComponent = GetAbilitySystemComponent())
	{
		if (Value.Get<bool>())
		{
			UE_LOG(LogTemp, Log, TEXT("Ability Local input pressed: %d"), InputID);
			AbilitySystemComponent->AbilityLocalInputPressed(InputID);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Ability Local input released: %d"), InputID);
			AbilitySystemComponent->AbilityLocalInputReleased(InputID);			
		}
	}
}
