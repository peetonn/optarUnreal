// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "Player/BasePlayerController.h"

#include <Kismet/GameplayStatics.h>

AMainGameMode::AMainGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // set default player controller class
    PlayerControllerClass = ABasePlayerController::StaticClass();
}

AMainGameMode::~AMainGameMode()
{
}

APlayerController*
AMainGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString & Options)
{
    // DLOG_TRACE("SpawnPlayerController: spawn options {}", TCHAR_TO_ANSI(*Options));
    FString clientPlatform = UGameplayStatics::ParseOption(Options, TEXT("ClientPlatform"));
    FString platform = clientPlatform;

    if (platform.IsEmpty())
        platform = UGameplayStatics::GetPlatformName();

    APlayerController *controller;

    if (platform.Equals(TEXT("Android")) ||
        platform.Equals(TEXT("IOS")))
    {
        controller = SpawnPlayerControllerCommon(InRemoteRole, FVector::ZeroVector, FRotator::ZeroRotator, ARPlayerControllerClass);
    }
    else // by default, spawn VR player controller
    {
        controller = SpawnPlayerControllerCommon(InRemoteRole, FVector::ZeroVector, FRotator::ZeroRotator, VRPlayerControllerClass);
    }

    return controller;
}

UClass*
AMainGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    ABasePlayerController *gamePlayerController = Cast<ABasePlayerController>(InController);

    if (gamePlayerController)
        return gamePlayerController->PawnClass;

    return NULL;
}

void
AMainGameMode::PreLogin(const FString & Options, const FString & Address,
                      const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage)
{
    // DLOG_DEBUG("PRE LOGIN: options {} address {} unique id {} error message {}",
    //            TCHAR_TO_ANSI(*Options), TCHAR_TO_ANSI(*Address),
    //            TCHAR_TO_ANSI(*(UniqueId.GetUniqueNetId()->ToString())),
    //            TCHAR_TO_ANSI(*ErrorMessage));

    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AMainGameMode::Login(UPlayer * NewPlayer, ENetRole InRemoteRole,
                                  const FString & Portal, const FString & Options,
                                  const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage)
{
    // TODO: clean this up or add logging
    // DLOG_DEBUG("LOGIN: new player. net role {} portal {} options {} unqiue id {} error message {}",
    //            (int)InRemoteRole,
    //            TCHAR_TO_ANSI(*Portal),
    //            TCHAR_TO_ANSI(*Options),
    //            TCHAR_TO_ANSI(*(UniqueId.GetUniqueNetId()->ToString())),
    //            TCHAR_TO_ANSI(*ErrorMessage));

    // APlayerController *controller = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

    // DLOG_DEBUG("LOGIN CREATED PLAYER CONTROLLER OF CLASS {}",
    //            TCHAR_TO_ANSI( *(controller->StaticClass()->GetDefaultObjectName().ToString())));

    return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);;
}

void AMainGameMode::PostLogin(APlayerController * NewPlayer)
{
    // DLOG_DEBUG("POST LOGIN PLAYER CONTROLLER {}",
    //            TCHAR_TO_ANSI( *(NewPlayer->StaticClass()->GetDefaultObjectName().ToString())));

    // TODO: specify player name / id here
    // if (NewPlayer->GetPlayerState<AGHPlayerState>())
    // {
    //     FString playerName("Silent Observer ");
    //     playerName.Append(FString::FromInt(GetNumPlayers()));
    //     NewPlayer->GetPlayerState<AGHPlayerState>()->SetPlayerName(playerName);
    // }

    Super::PostLogin(NewPlayer);
}
