using UnrealBuildTool;

public class OptMsgs : ModuleRules
{
    public OptMsgs(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore",
            "Rosbridge2Unreal", "jsoncons"
        });
    }
}
