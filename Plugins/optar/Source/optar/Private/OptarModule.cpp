// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OptarModule.h"

#include <string>

#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#if PLATFORM_ANDROID
#include "optar.hpp"
#endif

#define LOCTEXT_NAMESPACE "FoptarModule"


void FoptarModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if PLATFORM_ANDROID
//    char msg[256];
//    sprintf(msg, "%s", optar::getLibraryVersion());
//    
//    FText localizedText = FText::FromString(ANSI_TO_TCHAR(msg));
//    FMessageDialog::Open(EAppMsgType::Ok, localizedText);
#endif
}

void FoptarModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FoptarModule, optar)
