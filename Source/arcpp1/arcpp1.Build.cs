// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using System;

public class arcpp1 : ModuleRules
{
    private string ModulePath
    {
        get {
            return ModuleDirectory;
        }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

	public arcpp1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
        "InputCore", "UElibPNG", "AugmentedReality" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        //LoadOpenCv(Target);
	}

    public void LoadOpenCv(ReadOnlyTargetRules Target)
    {
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string IncludesPath = Path.Combine(ThirdPartyPath, "OpenCV", "include");
            string LibraryPath = Path.Combine(ThirdPartyPath, "OpenCV", "lib", "arm64-v8a");

            PublicIncludePaths.Add(IncludesPath);
            PublicLibraryPaths.Add(LibraryPath);
            PublicAdditionalLibraries.Add("opencv_java4");

            string RelAPLPath = Utils.MakePathRelativeTo(Path.Combine(ModulePath, "arcpp1_apl.xml"), Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", RelAPLPath));
        }
    }
}
