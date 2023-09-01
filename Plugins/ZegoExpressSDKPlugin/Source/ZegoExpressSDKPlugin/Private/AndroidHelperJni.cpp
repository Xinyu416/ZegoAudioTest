#if PLATFORM_ANDROID
#include "AndroidHelperJni.h"
#if PLATFORM_ANDROID
AndroidHelperJni::AndroidHelperJni() {}

AndroidHelperJni::~AndroidHelperJni() {}
void AndroidHelperJni::InitEnv() {

    UE_LOG(LogTemp, Log, TEXT("AndroidHelperJni::InitEnv"));
    if (JNIEnv *env = FAndroidApplication::GetJavaEnv()) {

        // ��ȡjvmָ��
        JavaVM *jvm;
        jint res = env->GetJavaVM(&jvm);
        if (res != JNI_OK) {
            UE_LOG(LogTemp, Log, TEXT("AndroidHelperJni::InitEnv, init jvm failed!"));
            return;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("AndroidHelperJni::InitEnv, init jvm successful!"));
        }

        // ��ȡcontext����
        jobject context = FAndroidApplication::GetGameActivityThis();
        if (context == NULL) {
            UE_LOG(LogTemp, Log, TEXT("AndroidHelperJni::InitEnv, init context failed!"));
            return;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("AndroidHelperJni::InitEnv, init context successful!"));
        }

        // �洢jvm��context��ָ��
        jvm_ = (void *)jvm;
        context_ = (void *)context;
    }
}
AndroidHelperJni *AndroidHelperJni::GetInstance() 
{ 
    static AndroidHelperJni inst;
    return &inst;
}

void *AndroidHelperJni::GetJvm() { return jvm_; }

void *AndroidHelperJni::GetContext() { return context_; }

#endif

#endif