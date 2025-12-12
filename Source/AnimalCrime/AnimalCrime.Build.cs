// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AnimalCrime : ModuleRules
{
	public AnimalCrime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "RenderCore", "RHI" });

        // 모듈 추가 (작업자: 나희영)
        PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem", "Niagara"});
        
        // 모듈 경로도 Include할 때 검색하도록 설정.
        PublicIncludePaths.AddRange(new string[] {
            "AnimalCrime"
        });

        // 기본 UI 모듈은 항상 포함
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // 에디터 전용 모듈은 에디터 빌드에서만 포함
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "EditorSubsystem", "LevelEditor" });
        }


        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
