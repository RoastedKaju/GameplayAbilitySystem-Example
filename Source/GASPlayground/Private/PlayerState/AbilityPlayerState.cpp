// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AbilityPlayerState.h"

#include "AdvancedAbilitySystemComponent.h"
#include "Attributes/IntermediateHealthAttributeSet.h"

AAbilityPlayerState::AAbilityPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAdvancedAbilitySystemComponent>(TEXT("Ability System Component"));
	HealthSet = CreateDefaultSubobject<UIntermediateHealthAttributeSet>(TEXT("Health Set"));
}

UAbilitySystemComponent* AAbilityPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
