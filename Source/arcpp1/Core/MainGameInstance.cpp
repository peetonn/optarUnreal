// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

#include <Misc/NetworkVersion.h>
#include <Engine/ObjectLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/LevelStreaming.h>

#include <string>
#include <sstream>


UMainGameInstance::UMainGameInstance()
:UGameInstance()
{
    connectionStatus_ = ConnectionStatus::Disconnected;
}

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitializer)
:UGameInstance(ObjectInitializer)
{
    connectionStatus_ = ConnectionStatus::Disconnected;
}

UMainGameInstance::~UMainGameInstance()
{
}

bool UMainGameInstance::HandleOpenCommand(const TCHAR * Cmd, FOutputDevice & Ar,
    UWorld * InWorld)
{
    updateConnectionStatus(ConnectionStatus::Connecting);

    return UGameInstance::HandleOpenCommand(Cmd, Ar, InWorld);
}

void UMainGameInstance::LoadComplete(const float LoadTime,
    const FString & MapName)
{
    UWorld *world = GetWorld();
    // check if we really connected
    if (world)
    {
        ENetMode mode = world->GetNetMode();
        switch (mode) {
            case NM_Standalone:
                updateConnectionStatus(ConnectionStatus::Disconnected);
                break;
            case NM_Client:
                updateConnectionStatus(ConnectionStatus::Connected);
                break;
            case NM_ListenServer:
                updateConnectionStatus(ConnectionStatus::Connected);
                break;
            case NM_DedicatedServer:
                updateConnectionStatus(ConnectionStatus::Connected);
                break;
            default:
                // do nothing
                break;
        }
    }
    else
        updateConnectionStatus(ConnectionStatus::Disconnected);
}


TArray<FString>
UMainGameInstance::GetAllMapNames() const
{
    auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
    ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Maps"));
    TArray<FAssetData> AssetDatas;
    ObjectLibrary->GetAssetDataList(AssetDatas);

    TArray<FString> Names = TArray<FString>();

    for (int32 i = 0; i < AssetDatas.Num(); ++i)
    {
        FAssetData& AssetData = AssetDatas[i];

        auto name = AssetData.AssetName.ToString();
        Names.Add(name);
    }

    Names.Sort();

    return Names;
}

bool
UMainGameInstance::isLevelLoaded(FString levelName) const
{
    ULevelStreaming* ls = UGameplayStatics::GetStreamingLevel(this, FName(levelName));

     if(ls != NULL)
     {
         return (ls->IsLevelLoaded() && ls->IsLevelVisible());
     }

    return false;
}

ConnectionStatus
UMainGameInstance::getConnectionStatus() const
{
    return connectionStatus_;
}

void
UMainGameInstance::setConnectionStatus(ConnectionStatus status)
{
    connectionStatus_ = status;
}

void
UMainGameInstance::updateConnectionStatus(ConnectionStatus newStatus)
{
    if (connectionStatus_ != newStatus)
    {
        ConnectionStatus oldStatus = connectionStatus_;
        connectionStatus_ = newStatus;

        // notify observers
        OnConnectionStatusUpdated.Broadcast(oldStatus, connectionStatus_);
    }
}
