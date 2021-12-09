// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelpersFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCPP1_API UHelpersFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helpers Library")
	static FString getRosBridgeServerIp();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helpers Library")
	static int getRosBridgeServerPort();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helpers Library")
	static bool getRosBridgeServerIsErrored();

};
