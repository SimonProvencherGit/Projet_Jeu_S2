#include "Music.h"
#include <mmsystem.h>
#include <iostream>
#include <thread>


Music::Music() : hWaveOut(NULL) {}

Music::~Music() {
    if (hWaveOut) {
        waveOutClose(hWaveOut);
    }
}

void Music::PlayMusicWithVolumeControl(LPCSTR lpszSound) {
    if (hWaveOut) {
        FadeOut(5000); // Fade out over 5 seconds
    }

    WAVEFORMATEX wfx;

    // Open the waveform-audio output device
    wfx.nSamplesPerSec = 44100; // Sample rate
    wfx.wBitsPerSample = 16;    // Sample size
    wfx.nChannels = 2;          // Number of channels
    wfx.cbSize = 0;             // Size of the structure
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR) {
        std::cerr << "Error opening waveform-audio output device." << std::endl;
        return;
    }

    // Play the sound asynchronously
    if (!PlaySoundA(lpszSound, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
        std::cerr << "Error playing sound." << std::endl;
    }
}

void Music::SetVolumeFadeout(int inputvolume) {
    // Ensure the volume is within the valid range
    if (inputvolume < 0) inputvolume = 0;
    if (inputvolume > 100) inputvolume = 100;

    // Convert the volume to the DWORD format
    DWORD dwVolume = (inputvolume * 0xFFFF / 100) | ((inputvolume * 0xFFFF / 100) << 16);
    waveOutSetVolume(hWaveOut, dwVolume);
}

void Music::SetVolume(int inputvolume) {
    // Ensure the volume is within the valid range
    if (inputvolume < 0) inputvolume = 0;
    if (inputvolume > 100) inputvolume = 100;

    // Convert the volume to the DWORD format
    DWORD dwVolume = (inputvolume * 0xFFFF / 100) | ((inputvolume * 0xFFFF / 100) << 16);
    waveOutSetVolume(hWaveOut, dwVolume);
    volume = inputvolume;
}

void Music::PlayMusicAsync(LPCSTR lpszSound) {
    std::thread(&Music::PlayMusicWithVolumeControl, this, lpszSound).detach();
}

void Music::FadeOut(int duration) {
    int tempvolume = volume;
    for (int downvolume = volume; downvolume >= 0; downvolume -= 1) {
        SetVolumeFadeout(downvolume);
        Sleep(duration / 100); // Adjust sleep time based on the duration
    }
    PlaySound(NULL, NULL, 0);
    volume = tempvolume;
    SetVolumeFadeout(volume); // Stop the sound
}