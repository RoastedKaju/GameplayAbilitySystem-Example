// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInputEnum.generated.h"

UENUM(BlueprintType)
enum class EAbilitySlotsEnum : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAbility,
	SecondaryAbility,
	UltimateAbility
};
