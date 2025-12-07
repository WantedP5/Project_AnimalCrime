// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AnimalCrime : ModuleRules
{
	public AnimalCrime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // ����
        PublicIncludePaths.AddRange(new[] { "AnimalCrime" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // 모듈 경로도 Include할 때 검색하도록 설정.
        PublicIncludePaths.AddRange(new string[] {
            "AnimalCrime"
        });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
