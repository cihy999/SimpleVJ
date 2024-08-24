// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimpleVJ : ModuleRules
{
	public SimpleVJ(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
