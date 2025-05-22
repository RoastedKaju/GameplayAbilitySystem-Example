// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GASPlayground : ModuleRules
{
	public GASPlayground(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
	}
}
