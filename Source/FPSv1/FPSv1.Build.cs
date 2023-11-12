// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSv1 : ModuleRules
{
	public FPSv1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "SlateCore" });
	}
}
