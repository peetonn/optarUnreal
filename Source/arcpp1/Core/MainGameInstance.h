// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"


UENUM(BlueprintType)
enum class ConnectionStatus : uint8 {
  Disconnected  UMETA(DisplayName = "Disconnected"),
  Connecting    UMETA(DisplayName = "Connecting"),
  Connected     UMETA(DisplayName = "Connected"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameInstanceConnectionStatusUpdate,
	ConnectionStatus, oldStatus, ConnectionStatus, newStatus);

/**
 * Game Instance class is persistent throughout the lifetime of the application.
 * It is created at app startup and destroyed on shutdown. It is not replicated.
 * This class can be used for utility helper functions.
 */
UCLASS()
class ARCPP1_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UMainGameInstance();
	UMainGameInstance(const FObjectInitializer& ObjectInitializer);
	~UMainGameInstance();

public:

	bool HandleOpenCommand(const TCHAR * Cmd,
	                           FOutputDevice & Ar,
	                           UWorld * InWorld) override;
	void LoadComplete(const float LoadTime,
	                  const FString & MapName) override;

	UFUNCTION(BlueprintCallable)
	ConnectionStatus getConnectionStatus() const;

	UFUNCTION(BlueprintCallable)
	void setConnectionStatus(ConnectionStatus status);

	UPROPERTY(BlueprintAssignable)
	FGameInstanceConnectionStatusUpdate OnConnectionStatusUpdated;

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllMapNames() const;

	UFUNCTION(BlueprintCallable)
	bool isLevelLoaded(FString levelName) const;

private:
	ConnectionStatus connectionStatus_;

	void updateConnectionStatus(ConnectionStatus newStatus);
};
