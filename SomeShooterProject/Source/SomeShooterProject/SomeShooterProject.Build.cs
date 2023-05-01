// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SomeShooterProject : ModuleRules
{
	public SomeShooterProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] 
		{ 
			"SomeShooterProject/Public/Core/Characters", 
			"SomeShooterProject/Public/Core/Characters/Components",
            "SomeShooterProject/Public/Core/Interactables",
            "SomeShooterProject/Public/Core/Weapons",
            "SomeShooterProject/Public/Core/UI",
            "SomeShooterProject/Public/Core/Animations",
            "SomeShooterProject/Public/Core/",
            "SomeShooterProject/Public/"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
