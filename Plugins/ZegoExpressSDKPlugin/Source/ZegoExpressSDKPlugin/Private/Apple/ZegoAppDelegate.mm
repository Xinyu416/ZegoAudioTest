#include "ZegoAppDelegate.h"
#include "Core.h"

#if PLATFORM_IOS
#include "IOSAppDelegate.h"
void ZegoAppDelegate::Init(){
    UE_LOG(LogTemp, Log, TEXT("ZegoAppDelegate::Init"));
    [[IOSAppDelegate GetDelegate] SetFeature:EAudioFeature::Playback Active:true];
    [[IOSAppDelegate GetDelegate] SetFeature:EAudioFeature::Record Active:true];
    [[IOSAppDelegate GetDelegate] SetFeature:EAudioFeature::BackgroundAudio Active:true];
}
#else
void ZegoAppDelegate::Init(){
    UE_LOG(LogTemp, Log, TEXT("ZegoAppDelegate::Init"));
}
#endif