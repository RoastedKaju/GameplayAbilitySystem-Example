// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AbilityPlayerState.generated.h"

class UAdvancedAbilitySystemComponent;
class UIntermediateHealthAttributeSet;

/**
 * @brief Player state holds the ability system component and attribute sets, it is replicated to all other clients.
 */
UCLASS()
class GASPLAYGROUND_API AAbilityPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAbilityPlayerState();

	/**
	 * @return Attached ability system component
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
	 *	@brief Not required but for C++ use you can use this to get the Health attribute.
	 * 
	 * @return Health attribute set.
	 */
	FORCEINLINE UIntermediateHealthAttributeSet* GetHealthAttributeSet() const { return HealthSet; }

protected:
	/// Ability system component pointer
	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly, Category="Ability System")
	TObjectPtr<UAdvancedAbilitySystemComponent> AbilitySystemComponent;

	/// Health attribute set
	UPROPERTY()
	TObjectPtr<UIntermediateHealthAttributeSet> HealthSet;
};
