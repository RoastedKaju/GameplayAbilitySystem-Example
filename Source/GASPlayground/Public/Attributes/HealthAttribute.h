// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HealthAttribute.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedEvent, UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class GASPLAYGROUND_API UHealthAttribute : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHealthAttribute();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	// Current health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Health, meta=(HideFromModifiers))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthAttribute, Health);

	// Upper limit for health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttribute, MaxHealth);
	
	// Meta attribute, Damage value calculated during GE
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthAttribute, Damage);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedEvent OnHealthChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
