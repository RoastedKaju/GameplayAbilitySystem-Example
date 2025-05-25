// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCharacter.h"

#include "AdvancedAbilitySystemComponent.h"
#include "Attributes/HealthAttribute.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Enums/AbilitySystemInputEnum.h"


// Sets default values
AAbilityCharacter::AAbilityCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAdvancedAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthSet = CreateDefaultSubobject<UHealthAttribute>(TEXT("HealthSet"));
}

// Called when the game starts or when spawned
void AAbilityCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Provide this character as both owner and avatar
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

// Called every frame
void AAbilityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AAbilityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Primary ability
		EnhancedInputComponent->BindAction(PrimaryInputAction, ETriggerEvent::Triggered, this, &AAbilityCharacter::OnPrimaryAbility);
		EnhancedInputComponent->BindAction(SecondaryInputAction, ETriggerEvent::Triggered, this, &AAbilityCharacter::OnSecondaryAbility);
	}
}

void AAbilityCharacter::OnPrimaryAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int>(EAbilitySlotsEnum::PrimaryAbility));
}

void AAbilityCharacter::OnSecondaryAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int>(EAbilitySlotsEnum::SecondaryAbility));
}

void AAbilityCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
	check(AbilitySystemComponent);

	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Local Input Pressed."));
		AbilitySystemComponent->AbilityLocalInputPressed(InputID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Local Input Released."));
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}

UAbilitySystemComponent* AAbilityCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
