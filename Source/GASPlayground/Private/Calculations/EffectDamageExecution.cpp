// Fill out your copyright notice in the Description page of Project Settings.


#include "Calculations/EffectDamageExecution.h"

void UEffectDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayTag DamagePramTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Parameters.Damage"), true);
	const float DamageRuntimeValue = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(DamagePramTag, true, 0.0f);
}
