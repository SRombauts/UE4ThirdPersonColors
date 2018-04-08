// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"

#include "ThirdPersonColorsCharacter.h"
#include "ThirdPersonColorsGameMode.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(50.f, false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnComponentBeginOverlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(SphereComponent);

	// Replicate for Network Multi-player
	SetReplicates(true);
}

// Called when an instance of this class is placed (in editor) or spawned, after all Blueprint Construction Scripts.
void APickup::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	// Select a random period for floating movement
	Period = { FMath::RandRange(0.5f, 5.f), FMath::RandRange(0.5f, 5.f), FMath::RandRange(0.5f, 5.f) };
	// TODO: also select a random color

	ApplyColor();
}

void APickup::ApplyColor()
{
	if (!MaterialInstance)
	{
		UMaterialInterface* Material = MeshComponent->GetMaterial(0);
		if (Material)
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
		}
	}
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue(ColorParameterName, ColorValue);
		MeshComponent->SetMaterial(0, MaterialInstance);
	}
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

void APickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AThirdPersonColorsCharacter* Character = Cast<AThirdPersonColorsCharacter>(OtherActor);
	if (Character)
	{
		if (Role == ENetRole::ROLE_Authority)
		{
			Character->CollectOnePickup();

			Destroy();

			// Count remaining pickups
			TArray<AActor*> RemainingPickups;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), RemainingPickups);
			if (0 == RemainingPickups.Num())
			{
				// TODO: single-player shortcut for GameMode -Multicast-> GameState -> Character
				Character->CompleteMission();

				AThirdPersonColorsGameMode* GameMode = Cast<AThirdPersonColorsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GameMode)
				{
					GameMode->CompleteMission();
				}
			}
		}

		// Change body color
		Character->ChangeColor(ColorValue);

		PlayEffects(Character->GetActorLocation());
	}
}

void APickup::PlayEffects(const FVector& Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, Location);
	UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
}

// Called every frame: uses a sinusoid mathematical function to loop periodically across time
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Offset;
	Offset.X = (FMath::Sin((RunningTime + DeltaTime) * Period.X) - FMath::Sin(RunningTime * Period.X)) * Scale.X;
	Offset.Y = (FMath::Sin((RunningTime + DeltaTime) * Period.Y) - FMath::Sin(RunningTime * Period.Y)) * Scale.Y;
	Offset.Z = (FMath::Sin((RunningTime + DeltaTime) * Period.Z) - FMath::Sin(RunningTime * Period.Z)) * Scale.Z;
	RunningTime += DeltaTime;

	AddActorLocalOffset(Offset);
}
