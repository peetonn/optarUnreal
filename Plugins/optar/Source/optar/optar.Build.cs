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
            });

        PrivateIncludePaths.AddRange(
            new string[] {
            });

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "optarLibrary",
                "Projects",
                "AugmentedReality"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "RHI"
            });
        // add platform-specific dependency modules
        if (Target.Platform == UnrealTargetPlatform.IOS ||
            Target.Platform == UnrealTargetPlatform.Mac)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "AppleARKit",
                    "AppleImageUtils"
                });
        }

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            });

        LoadOptar(Target);
    }

    public void LoadOptar(ReadOnlyTargetRules Target)
    {
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "optarLibrary", "include"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "optarLibrary", "src"));

        if (Target.Platform == UnrealTargetPlatform.Android)
        {

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "optar_APL.xml")));

            string LibraryPath = Path.Combine(ThirdPartyPath, "optarLibrary", "Android", "libs", "arm64-v8a");

            PublicLibraryPaths.Add(LibraryPath);
            PublicAdditionalLibraries.Add("optar");
        }
        if (Target.Platform == UnrealTargetPlatform.IOS)
        {

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);

            string LibraryPath = Path.Combine(ThirdPartyPath, "optarLibrary", "ios", "build", "arm64");

            PublicLibraryPaths.Add(LibraryPath);
            PublicAdditionalLibraries.Add("optar");
            PublicAdditionalLibraries.Add("ros");
            PublicAdditionalLibraries.Add("opencv2");
            PublicAdditionalLibraries.Add("boost");
            PublicAdditionalLibraries.Add("log4cxx");
            PublicAdditionalLibraries.Add("expat");
            PublicAdditionalLibraries.Add("iconv");

            PublicSystemLibraryPaths.Add("/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/lib/swift/");
            PublicSystemLibraryPaths.Add("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift");
            PublicSystemLibraryPaths.Add("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/iphoneos");
            PublicSystemLibraryPaths.Add("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift-5.0/iphoneos");
        }
    }
}
