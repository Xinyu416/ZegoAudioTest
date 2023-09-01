#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"

class AndroidHelperJni {
  public:
    AndroidHelperJni();
    ~AndroidHelperJni();
    static AndroidHelperJni *GetInstance();

    void InitEnv();
    void* GetJvm();
    void* GetContext();
  private:
    void *jvm_;
    void *context_;
};
#endif
