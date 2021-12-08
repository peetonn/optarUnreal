// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * Custom game mode class.
 * It is server-only (for single-player it means it is just created on startup).
 * In multiplayer, clients don't have game modes.
 * This custom game mode stores pointers to specific player controller classes
 * for cross-platform clients -- i.e. AR player controller and VR player
 * controller. When new client logs in to a multiplayer session, login Options
 * are checked for the ClientPlatform argument which specifies the platform
 * client is running on. For "IOS" and "Android" values, AR Player Controller is
 * spawned, otherwise -- VR Player Controller is used.
 * Values for player controller classes are not set here. One must subclass from
 * this class in blueprints and set proper values.
 */
UCLASS()
class ARCPP1_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode(const FObjectInitializer& ObjectInitializer);
	~AMainGameMode();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerController> VRPlayerControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerController> ARPlayerControllerClass;

private:
	// AR-/VR-specific player controller spawning handling
    APlayerController *SpawnPlayerController(ENetRole InRemoteRole,
		const FString & Options) override;
    UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual void PreLogin(const FString & Options, const FString & Address,
                  const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;
    virtual APlayerController * Login(UPlayer * NewPlayer, ENetRole InRemoteRole,
                                      const FString & Portal, const FString & Options,
                                      const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;
    virtual void PostLogin(APlayerController * NewPlayer) override;

};
