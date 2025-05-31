// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/IntermediateHealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UIntermediateHealthAttributeSet::UIntermediateHealthAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitEnergy(50.0f);
	InitMaxEnergy(50.0f);
}

void UIntermediateHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UIntermediateHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
                                                          float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(this, OldValue, NewValue);
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		// If Health is greater than the new value of Max Health
		if (GetHealth() > NewValue)
		{
			auto AbilitySystemComponent = GetOwningAbilitySystemComponentChecked();
			if (AbilitySystemComponent != nullptr)
			{
				// We do this so it updates the UI by called pre-and-post attribute change, calling Set Health would bypass those.
				AbilitySystemComponent->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
			}
		}

		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
	}
}

void UIntermediateHealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageValue = GetDamage();
		const float OldHealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.0f, MaxHealthValue);

		if (OldHealthValue != NewHealthValue)
		{
			SetHealth(NewHealthValue);
		}

		SetDamage(0.0f);
	}
}

void UIntermediateHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UIntermediateHealthAttributeSet, Health, OldValue);
}

void UIntermediateHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UIntermediateHealthAttributeSet, MaxHealth, OldValue);
}

void UIntermediateHealthAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UIntermediateHealthAttributeSet, Energy, OldValue);
}

void UIntermediateHealthAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UIntermediateHealthAttributeSet, MaxEnergy, OldValue);
}

void UIntermediateHealthAttributeSet::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UIntermediateHealthAttributeSet, Health);
	DOREPLIFETIME(UIntermediateHealthAttributeSet, MaxHealth);

	DOREPLIFETIME(UIntermediateHealthAttributeSet, Energy);
	DOREPLIFETIME(UIntermediateHealthAttributeSet, MaxEnergy);
}
