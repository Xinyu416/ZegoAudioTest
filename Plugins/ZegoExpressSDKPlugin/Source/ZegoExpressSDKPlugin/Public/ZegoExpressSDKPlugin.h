// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "AndroidHelperJni.h"

class FZegoExpressSDKPluginModule : public IModuleInterface {
  public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
  private:
    void* LibraryHandle;
};
