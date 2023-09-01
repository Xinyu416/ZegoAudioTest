// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnrealBuildTool;

public class ZegoExpressSDKPlugin : ModuleRules
{
	public ZegoExpressSDKPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        string CommonLibPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/ZegoExpressSDKPluginLibrary/"));

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
                "ApplicationCore"
			});

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string OSVersion = "x64";
            string LibPath = Path.Combine(CommonLibPath, "Windows", "x64");
            string libFilePath = Path.Combine(LibPath, "ZegoExpressEngine.lib");
            PublicAdditionalLibraries.Add(libFilePath);

            string binariesDir = Path.Combine(ModuleDirectory, "../../Binaries", Target.Platform.ToString());
            if (!Directory.Exists(binariesDir))
                Directory.CreateDirectory(binariesDir);

            string[] dllNames = new string[] {
                    "ZegoExpressEngine.dll",
                };

            foreach (string dllName in dllNames)
            {
                RuntimeDependencies.Add("$(TargetOutputDir)/" + dllName, Path.Combine(LibPath, dllName));
            }

            try
            {
                foreach (string dllName in dllNames)
                {
                    System.Console.WriteLine("src dll=" + dllName + " dst dir=" + binariesDir);
                    File.Copy(Path.Combine(LibPath, dllName), Path.Combine(binariesDir, dllName), true);
                }
            }
            catch (Exception e)
            {
                System.Console.WriteLine("copy dll exception,maybe have exists,err=", e.ToString());
            }
            PublicDefinitions.Add("WIN32_LEAN_AND_MEAN");
            PublicDefinitions.Add("WINDOWS_IGNORE_PACKING_MISMATCH");
            string[] pathGroup = new string[1];
            pathGroup[0] = Path.Combine(CommonLibPath, "Windows", "x64", "include");
            PublicIncludePaths.AddRange(pathGroup);
            PrivateIncludePaths.AddRange(pathGroup);
        }
#if UE_5_0_OR_LATER
#else
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            string OSVersion = "x86";
            string LibPath = Path.Combine(CommonLibPath, "Windows", OSVersion);
            string libFilePath = Path.Combine(LibPath, "ZegoExpressEngine.lib");
            PublicAdditionalLibraries.Add(libFilePath);

            string binariesDir = Path.Combine(ModuleDirectory, "../../Binaries", Target.Platform.ToString());
            if (!Directory.Exists(binariesDir))
                Directory.CreateDirectory(binariesDir);

            string[] dllNames = new string[] {
                    "ZegoExpressEngine.dll",
                };

            foreach (string dllName in dllNames)
            {
                RuntimeDependencies.Add("$(TargetOutputDir)/" + dllName, Path.Combine(LibPath, dllName));
            }

            try
            {
                foreach (string dllName in dllNames)
                {
                    System.Console.WriteLine("src dll=" + dllName + " dst dir=" + binariesDir);
                    File.Copy(Path.Combine(LibPath, dllName), Path.Combine(binariesDir, dllName), true);
                }
            }
            catch (Exception e)
            {
                System.Console.WriteLine("copy dll exception,maybe have exists,err=", e.ToString());
            }
            PublicDefinitions.Add("WIN32_LEAN_AND_MEAN");
            PublicDefinitions.Add("WINDOWS_IGNORE_PACKING_MISMATCH");
            string[] pathGroup = new string[1];
            pathGroup[0] = Path.Combine(CommonLibPath, "Windows", "x86", "include");
            PublicIncludePaths.AddRange(pathGroup);
            PrivateIncludePaths.AddRange(pathGroup);
        }
#endif
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // 添加依赖的模块名称
            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

            string[] pathGroup = new string[4];
            // armabi-v7a
            string LibPath = Path.Combine(CommonLibPath, "Android", "armeabi-v7a");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libZegoExpressEngine.so"));
            pathGroup[0] = Path.Combine(LibPath, "include");

            // arm-v8a
            LibPath = Path.Combine(CommonLibPath, "Android", "arm64-v8a");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libZegoExpressEngine.so"));
            pathGroup[1] = Path.Combine(LibPath, "include");

            // x86
            LibPath = Path.Combine(CommonLibPath, "Android", "x86");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libZegoExpressEngine.so"));
            pathGroup[2] = Path.Combine(LibPath, "include");

            // x64
            LibPath = Path.Combine(CommonLibPath, "Android", "x86_64");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libZegoExpressEngine.so"));
            pathGroup[3] = Path.Combine(LibPath, "include");

            PublicIncludePaths.AddRange(pathGroup);
            PrivateIncludePaths.AddRange(pathGroup);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "plugin.xml"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            System.Console.WriteLine("enter macoc process!");
            string LibPath = Path.Combine(CommonLibPath, "Mac", "ZegoExpressEngine.xcframework", "macos-arm64_x86_64");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libZegoExpressEngine.dylib"));
            //PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "libZegoExpressEngine.dylib"));
            //RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ZegoExpressSDKPluginLibrary/Mac/ZegoExpressEngine.xcframework/macos-arm64_x86_64/libZegoExpressEngine.dylib");
            string[] pathGroup = new string[1];
            pathGroup[0] = Path.Combine(LibPath, "Headers");
            PublicIncludePaths.AddRange(pathGroup);
            PrivateIncludePaths.AddRange(pathGroup);
            PublicSystemLibraries.AddRange(
                new string[]{
                    "iconv",
                    "resolv",
                    "z",
                    "c++",
                    "System",
                    "objc"
                });
            PublicFrameworks.AddRange(
                new string[] {
                    "AudioToolbox",
                    "AudioUnit",
                    "AVFoundation",
                    "AppKit",
                    "CoreAudio",
                    "CoreGraphics",
                    "CoreMedia",
                    "CoreVideo",
                    "OpenGL",
                    "VideoDecodeAcceleration",
                    "VideoToolbox",
                    "CoreImage",
                    "Accelerate",
                    "Security",
                    "SystemConfiguration",
                    "QuartzCore",
                    "IOKit",
                    "IOSurface",
                    "ApplicationServices",
                    "CoreFoundation",
                    "Foundation",
                    "ImageIO",
                    "CFNetwork",
                    "CoreFoundation",
                    "CoreText",
                    "Foundation",

                });
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            System.Console.WriteLine("enter ios process!");
            string LibZipPath = Path.Combine(CommonLibPath, "iOS", "ZegoExpressEngine.framework.zip");
            string[] pathGroup = new string[1];
            pathGroup[0] = Path.Combine(CommonLibPath, "iOS", "Headers");
            PublicIncludePaths.AddRange(pathGroup);
            PrivateIncludePaths.AddRange(pathGroup);
            PublicSystemLibraries.AddRange(
                new string[]{
                    "iconv",
                    "resolv",
                    "z",
                    "lzma",
                    "bz2",
                    "c++",
                    "System",
                    "objc"
                });
            PublicFrameworks.AddRange(
                new string[] {
                    "AudioToolbox",
                    "AVFoundation",
                    "CoreGraphics",
                    "CoreMedia",
                    "CoreVideo",
                    "GLKit",
                    "OpenGLES",
                    "QuartzCore",
                    "UIKit",
                    "VideoToolbox",
                    "CoreImage",
                    "Metal",
                    "CoreMotion",
                    "Accelerate",
                    "CoreTelephony",
                    "Security",
                    "SystemConfiguration",
                    "CoreLocation",
                    "ReplayKit",
                    "CFNetwork",
                    "CoreFoundation",
                    "CoreText",
                    "Foundation",

                });
#if UE_4_26_OR_LATER
            PublicAdditionalFrameworks.Add(new Framework("ZegoExpressEngine", LibZipPath, "Modules/module.modulemap", true));
#else
            PublicAdditionalFrameworks.Add(new Framework("ZegoExpressEngine", LibZipPath, "Modules/module.modulemap"));
#endif
        }
    }
}
