﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/HealthAttribute.h"
#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

UHealthAttribute::UHealthAttribute()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

void UHealthAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UHealthAttribute::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(this, OldValue, NewValue);
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
	}
}

void UHealthAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// In Post Gameplay Effect we check if the evaluated attribute is damage attribute
	// If Yes we Calculate the new health by subtracting damage from current health
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageValue = GetDamage();
		const float OldHealth = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealth - DamageValue, 0.0f, MaxHealthValue);

		if (OldHealth != NewHealthValue)
		{
			SetHealth(NewHealthValue);
			
			// Calculate the actual damage applied that respects the min and max values.
			const float ActualDamageNumber = OldHealth - NewHealthValue;
			if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
			{
				// Broadcast the damage number gameplay cue on the owning actor. Triggered on server, executes on all clients.
				const FGameplayTag DamageCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.DamageNumber", true);
				FGameplayCueParameters DamageCueParameters;
				DamageCueParameters.NormalizedMagnitude = 1.0f;
				DamageCueParameters.RawMagnitude = ActualDamageNumber;
				OwningAbilitySystemComponent->ExecuteGameplayCue(DamageCueTag, DamageCueParameters);
			}
		}

		// Clear the meta attribute
		SetDamage(0.0f);
	}
}

void UHealthAttribute::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttribute, Health, OldValue);
	const float OldHealth = OldValue.GetCurrentValue();
	const float NewHealth = GetHealth();
	OnHealthChanged.Broadcast(this, OldHealth, NewHealth);
}

void UHealthAttribute::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttribute, MaxHealth, OldValue);

	// When max health changes broadcast the "health" again with old and new values same so we can update the percentage in progress bar
	const float CurrentHealth = GetHealth();
	OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
}

void UHealthAttribute::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAttribute, Health);
	DOREPLIFETIME(UHealthAttribute, MaxHealth);
}
