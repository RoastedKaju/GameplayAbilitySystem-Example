// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "IntermediateHealthAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntermediateOnAttributeChanged, UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

/**
 * @brief Attribute set related to health and damage, it is better to divide attribute sets based on common data
 */
UCLASS()
class GASPLAYGROUND_API UIntermediateHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UIntermediateHealthAttributeSet();

	/**
	 * @brief Runs before changes to attributes, good place to clamp values that are modified without using meta attributes.
	 * 
	 * @param Attribute Current attribute that is changing.
	 * @param NewValue New value.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	/**
	 * @brief Runs after changes to attributes, good place to broadcast to let other systems know of the updates to attributes
	 * 
	 * @param Attribute Current attribute that is changing.
	 * @param OldValue Old Value before change.
	 * @param NewValue New value.
	 */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	/**
	 * @brief Runs after a gameplay effect is executed, good place to clamp and reset meta attributes.
	 * 
	 * @param Data Contains the attribute Data.EvaluatedData.Attribute
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/// Current health, hidden from blueprints, use meta damage attribute to make changes to this attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category = "Attributes", meta=(HideFromModifiers))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UIntermediateHealthAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UIntermediateHealthAttributeSet, MaxHealth);

	/// Meta attribute that is only set before applying damage and then reset in post gameplay effect.
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UIntermediateHealthAttributeSet, Damage);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintAssignable)
	FIntermediateOnAttributeChanged OnHealthChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
