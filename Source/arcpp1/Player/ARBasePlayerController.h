// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "ARBasePlayerController.generated.h"

/**
 * AR Player Controller for custom C++ code.
 */
UCLASS()
class ARCPP1_API AARBasePlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    FTransform ArAlignment;

    UFUNCTION(BlueprintCallable)
    void setArAlignment(FTransform transform);

    UFUNCTION(BlueprintCallable)
    void updateArAlignment(FTransform transform);

};
