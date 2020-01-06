// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class optar : ModuleRules
{
    private string ModulePath
    {
        get {
            return ModuleDirectory;
        }
    }
        
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
    }
    
	public optar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			});
				
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			});
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
				"optarLibrary",
				"Projects",
                "AugmentedReality",
                "GoogleARCoreBase"
				// ... add other public dependencies that you statically link with here ...
			});
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			});
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			});
        
        LoadOptar(Target);
	}
    
    public void LoadOptar(ReadOnlyTargetRules Target)
    {
        string IncludesPath = Path.Combine(ThirdPartyPath, "optarLibrary", "source");
        PublicIncludePaths.Add(IncludesPath);
        
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "optar_APL.xml")));
            
            string LibraryPath = Path.Combine(ThirdPartyPath, "optarLibrary", "Android", "libs", "arm64-v8a");
               
            PublicLibraryPaths.Add(LibraryPath);
            PublicAdditionalLibraries.Add("optar");
        }
    }
}
