// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/HealthAttribute.h"

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
