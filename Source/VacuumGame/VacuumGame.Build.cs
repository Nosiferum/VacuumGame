// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VacuumGame : ModuleRules
{
	public VacuumGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
