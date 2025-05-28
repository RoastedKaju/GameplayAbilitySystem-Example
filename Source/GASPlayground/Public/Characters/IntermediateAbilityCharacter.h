// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "IntermediateAbilityCharacter.generated.h"

class UIntermediateHealthAttributeSet;
class UInputAction;

/**
 * @brief Contains Input actions and functions, implements ability system interface so we can directly access it without going through player state.
 */
UCLASS()
class GASPLAYGROUND_API AIntermediateAbilityCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIntermediateAbilityCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief This function runs on server after the possession takes place, good place to execute server related code.
	 * 
	 * @param NewController The Controller taking command.
	 */
	virtual void PossessedBy(AController* NewController) override;
	/**
	 * @brief Once the Player state has been replicated to the clients this function is called, good place to run client side code after server.
	 */
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ConfirmAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> SecondaryAction;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * @brief Recommend even if the ASC exists on Player state, this will make the gameplay debugger work and make it easy to access ASC
	 * 
	 * @return Ability system component from Player state.
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category="Input")
	void OnConfirmInput(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnCancelInput(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category="Input")
	void OnPrimaryInput(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnSecondaryInput(const FInputActionValue& Value);

	/**
	 * @brief Sends the input ID of action based on the pressed and released boolean state of input.
	 * 
	 * @param Value Input action value, can be bool in case of pressed and released.
	 * @param InputID Input ID taken from the Enum for Ability input to easily match the Input ID of Ability.
	 */
	UFUNCTION(BlueprintCallable, Category="Input")
	void SendAbilityLocalInput(const FInputActionValue& Value, int InputID);
};
