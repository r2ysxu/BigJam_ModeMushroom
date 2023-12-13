// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BigJam01 : ModuleRules
{
	public BigJam01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });
	}
}
