#include "ZegoAppDelegate.h"

ZegoAppDelegate::ZegoAppDelegate(){

}

ZegoAppDelegate& ZegoAppDelegate::Instance(){
    static ZegoAppDelegate __instance;
    return __instance;
}

#if !defined(__APPLE__)
void ZegoAppDelegate::Init(){

}
#endif