#pragma once

class ZegoAppDelegate{
private:    
    ZegoAppDelegate();
public:
    static ZegoAppDelegate& Instance();
public:
    void Init();
};