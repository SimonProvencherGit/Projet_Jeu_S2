#ifndef MUSIC_H
#define MUSIC_H

#include <windows.h>
#pragma comment(lib, "winmm.lib")

class Music {
public:
    Music();
    ~Music();
    void StopMusic();
    void PlayMusicWithVolumeControl(LPCSTR lpszSound);
    void SetVolumeFadeout(int volume);
    void SetVolume(int volume);
    void PlayMusicAsync(LPCSTR lpszSound);
private:
    HWAVEOUT hWaveOut;
    int volume = 50;

    void FadeOut(int duration);
};

#endif