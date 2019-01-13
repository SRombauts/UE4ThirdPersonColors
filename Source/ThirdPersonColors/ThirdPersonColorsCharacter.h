// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonColorsCharacter.generated.h"

UCLASS(config=Game)
class AThirdPersonColorsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AThirdPersonColorsCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

	void ApplyColor();

	void ChangeColor(const FLinearColor& InColorValue);

	void CollectOnePickup();

	void CompleteMission();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	float BaseTurnRate;

	/* Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	float BaseLookUpRate;

	// Number of Pickups collected
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	int32 PickupCount = 0;

	// The sound of victory
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* MissionCompleteSound;

	UFUNCTION(BlueprintImplementableEvent, Category="Gameplay")
	void OnPickupCollected();

	UFUNCTION(BlueprintImplementableEvent, Category="Gameplay")
	void OnMissionCompleted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void StartSprinting();
	void StopSprinting();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	FLinearColor ColorValue = FLinearColor(1.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FName ColorParameterName = TEXT("BodyColor");

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* MaterialInstance;
};

