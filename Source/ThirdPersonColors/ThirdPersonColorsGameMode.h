// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonColorsGameMode.generated.h"

UCLASS(minimalapi)
class AThirdPersonColorsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThirdPersonColorsGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The UMG Widget class to use for the HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// The instance of the HUD
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class UUserWidget* CurrentWidget;
};



