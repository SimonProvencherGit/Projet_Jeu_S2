#ifndef SFX_H
#define SFX_H

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

class SFX {
public:
    SFX();
    ~SFX();
    void PlaySFXWithVolumeControl(LPCSTR lpszSound);
    void PlaySFXAsync(LPCSTR lpszSound);

private:
    std::vector<HWAVEOUT> waveOutPool;
    std::mutex poolMutex;
    HWAVEOUT getAvailableWaveOut();
    void releaseWaveOut(HWAVEOUT hWaveOut);
};

#endif
#pragma once
