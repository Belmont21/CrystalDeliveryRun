// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrystalDeliveryRun : ModuleRules
{
	public CrystalDeliveryRun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore"
			
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"CrystalDeliveryRun",
			"CrystalDeliveryRun/Variant_Platforming",
			"CrystalDeliveryRun/Variant_Platforming/Animation",
			"CrystalDeliveryRun/Variant_Combat",
			"CrystalDeliveryRun/Variant_Combat/AI",
			"CrystalDeliveryRun/Variant_Combat/Animation",
			"CrystalDeliveryRun/Variant_Combat/Gameplay",
			"CrystalDeliveryRun/Variant_Combat/Interfaces",
			"CrystalDeliveryRun/Variant_Combat/UI",
			"CrystalDeliveryRun/Variant_SideScrolling",
			"CrystalDeliveryRun/Variant_SideScrolling/AI",
			"CrystalDeliveryRun/Variant_SideScrolling/Gameplay",
			"CrystalDeliveryRun/Variant_SideScrolling/Interfaces",
			"CrystalDeliveryRun/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
