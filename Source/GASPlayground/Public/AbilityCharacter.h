// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilityCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UAdvancedAbilitySystemComponent;
class UHealthAttribute;

UCLASS(Config=Game)
class GASPLAYGROUND_API AAbilityCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAbilityCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Input")
	void OnPrimaryAbility(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnSecondaryAbility(const FInputActionValue& Value);

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAdvancedAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> PrimaryInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> SecondaryInputAction;
	
protected:
	UPROPERTY()
	TObjectPtr<UHealthAttribute> HealthSet;
};
