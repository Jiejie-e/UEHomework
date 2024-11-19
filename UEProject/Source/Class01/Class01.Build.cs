// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Class01 : ModuleRules
{
	public Class01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
