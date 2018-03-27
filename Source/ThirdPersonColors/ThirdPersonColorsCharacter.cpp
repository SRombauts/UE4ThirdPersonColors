// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonColorsCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

//////////////////////////////////////////////////////////////////////////
// AThirdPersonColorsCharacter

AThirdPersonColorsCharacter::AThirdPersonColorsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f; // Default is 420.f
	GetCharacterMovement()->AirControl = 0.2f; // Default is 0.05f

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
// At the height of Character's eyes, just a bit to the right to better see
//	CameraBoom->SocketOffset = FVector(0.f, 20.f, 118.f); // This would offset the socket, that is the camera, not the attachment point on the character (which is not nice when the camera get closer)
	CameraBoom->AddLocalOffset(FVector(0.f, 20.f, 118.f)); // This offset the attachment point into the character, so that we see the head when the camera get closer

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->RelativeRotation = FRotator(-15.f, 0.f, 0.f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Called when an instance of this class is placed (in editor) or spawned, after all Blueprint Construction Scripts.
void AThirdPersonColorsCharacter::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	ApplyColor();
}

void AThirdPersonColorsCharacter::ApplyColor()
{
	USkeletalMeshComponent* Mesh = GetMesh();
	if (Mesh)
	{
		if (!MaterialInstance)
		{
			UMaterialInterface* Material = Mesh->GetMaterial(0);
			if (Material)
			{
				MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
			}
		}
		if (MaterialInstance)
		{
			MaterialInstance->SetVectorParameterValue(ColorParameterName, ColorValue);
			Mesh->SetMaterial(0, MaterialInstance);
		}
	}
}

void AThirdPersonColorsCharacter::ChangeColor(const FLinearColor& InColorValue)
{
	ColorValue = InColorValue;
	ApplyColor();
}

// Called when the game starts or when spawned
void AThirdPersonColorsCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonColorsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonColorsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonColorsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonColorsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonColorsCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AThirdPersonColorsCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AThirdPersonColorsCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AThirdPersonColorsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonColorsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
