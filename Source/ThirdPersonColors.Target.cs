// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonColorsTarget : TargetRules
{
	public ThirdPersonColorsTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;

		Type = TargetType.Game;
		ExtraModuleNames.Add("ThirdPersonColors");
	}
}
