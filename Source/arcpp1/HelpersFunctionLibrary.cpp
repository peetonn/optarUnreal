// Fill out your copyright notice in the Description page of Project Settings.


#include "HelpersFunctionLibrary.h"
#include "IRosbridge2Unreal.h"

FString UHelpersFunctionLibrary::getRosBridgeServerIp()
{
	if (IRosbridge2Unreal::IsAvailable())
	{
		return IRosbridge2Unreal::Get().GetSettings()->IP;
	}
	return FString("n/a");
}

int UHelpersFunctionLibrary::getRosBridgeServerPort()
{
	if (IRosbridge2Unreal::IsAvailable())
	{
		return IRosbridge2Unreal::Get().GetSettings()->Port;
	}
	return 0;
}

bool UHelpersFunctionLibrary::getRosBridgeServerIsErrored()
{
	if (IRosbridge2Unreal::IsAvailable())
	{
		return IRosbridge2Unreal::Get().HasFailedConnection();
	}
	return true;
}