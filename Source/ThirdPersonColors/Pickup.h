// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class THIRDPERSONCOLORS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual void OnConstruction(const FTransform& Transform) override;

	void ApplyColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PickupFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FLinearColor ColorValue = FLinearColor(1.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly)
	FName ColorParameterName = TEXT("Color");

	// Period of the floating movement, in seconds
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floating Movement")
	FVector Period; // set OnConstruction()

	// Scale of the movement, in units (centimeters)
	UPROPERTY(EditAnywhere, Category = "Floating Movement")
	FVector Scale = { 10.0f, 10.0f, 20.0f };

	float RunningTime = 0.0f;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* MaterialInstance;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayEffects(const FVector& Location);
};
