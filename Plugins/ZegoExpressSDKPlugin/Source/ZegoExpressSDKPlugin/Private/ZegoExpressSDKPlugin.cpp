// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZegoExpressSDKPlugin.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "ZegoExpressSDK.h"

#define LOCTEXT_NAMESPACE "FZegoExpressSDKPluginModule"

void FZegoExpressSDKPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("ZegoExpressSDKPlugin")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/ZegoExpressSDKPluginLibrary/Mac/ZegoExpressEngine.xcframework/macos-arm64_x86_64/libZegoExpressEngine.dylib"));
#endif // PLATFORM_MAC

	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (LibraryHandle)
	{
		//successful
		UE_LOG(LogTemp, Log, TEXT("load third library successful!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("load third library failed!"));
	}
	ZegoAppDelegate::Instance().Init();
}

void FZegoExpressSDKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPlatformProcess::FreeDllHandle(LibraryHandle); 
	LibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FZegoExpressSDKPluginModule, ZegoExpressSDKPlugin)
