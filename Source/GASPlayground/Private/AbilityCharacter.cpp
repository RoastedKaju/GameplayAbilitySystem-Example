// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCharacter.h"

#include "AdvancedAbilitySystemComponent.h"
#include "Attributes/HealthAttribute.h"
#include "EnhancedInputComponent.h"


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

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComp && AbilitySystemComponent)
	{
		const FTopLevelAssetPath EnumName("/Script/GASPlayground.EAbilitySlotsEnum");
		FGameplayAbilityInputBinds Binds("ConfirmInput", "CancelInput", EnumName);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(EnhancedInputComp, Binds);
	}
}

UAbilitySystemComponent* AAbilityCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
