// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

ABasePlayerController::ABasePlayerController()
 : APlayerController()
 {
     bReplicates = true;
 }

ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer)
 : APlayerController(ObjectInitializer)
{
    bReplicates = true;
}

ABasePlayerController::~ABasePlayerController()
{
}
