#ifndef SFX_H
#define SFX_H
#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#pragma comment(lib, "winmm.lib")

class SFX {
public:
    SFX();
    ~SFX();
    void PlaySFXAsync(LPCSTR lpszSound);
    void setVolume(int volume);

private:
    std::vector<HWAVEOUT> waveOutPool;
    std::mutex poolMutex;
    HWAVEOUT getAvailableWaveOut();
    void releaseWaveOut(HWAVEOUT hWaveOut);
    int volume = 100;
};

#endif
#pragma once