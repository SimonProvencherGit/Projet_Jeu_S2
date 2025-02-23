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

//Code utilise (https://learn.microsoft.com/en-us/windows/win32/multimedia/using-the-waveformatex-structure)
void Music::PlayMusicWithVolumeControl(LPCSTR lpszSound) {
    if (hWaveOut) {
        FadeOut(5000); // Un effet de fade pendant 5 secondes quand la prochain musique commence
    }
    // Creer une structure de format d'onde appler wfx
    WAVEFORMATEX wfx;

    wfx.nSamplesPerSec = 44100; // Sample rate
    wfx.wBitsPerSample = 16;    // Sample size
    wfx.nChannels = 2;          // Number of channels
    wfx.cbSize = 0;             // Size of the structure
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR) {
        // Erreur si le format de la musique n'est pas supporte.
        std::cerr << "Error peut pas ouvrir ce format" << std::endl;
        return;
    }

    // Jouer le son Async en boucle
    if (!PlaySoundA(lpszSound, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
        std::cerr << "Erreur peut pas jouer la musique" << std::endl;
    }
}

void Music::SetVolumeFadeout(int inputvolume) {
    //Changer le volume avec un range de 0 a 100
    if (inputvolume < 0) inputvolume = 0;
    if (inputvolume > 100) inputvolume = 100;

    // Converter les valeurs de 0 a 100 en DWORD pour le volume de la musique (https://learn.microsoft.com/en-us/previous-versions/dd743680(v=vs.85))
    DWORD dwVolume = (inputvolume * 0xFFFF / 100) | ((inputvolume * 0xFFFF / 100) << 16);
    waveOutSetVolume(hWaveOut, dwVolume);
}

void Music::SetVolume(int inputvolume) {
    // Changer volume de 0 a 100
    if (inputvolume < 0) inputvolume = 0;
    if (inputvolume > 100) inputvolume = 100;

    // Converter les valeurs de 0 a 100 en DWORD pour le volume de la musique (https://learn.microsoft.com/en-us/previous-versions/dd743680(v=vs.85))
    DWORD dwVolume = (inputvolume * 0xFFFF / 100) | ((inputvolume * 0xFFFF / 100) << 16);
    waveOutSetVolume(hWaveOut, dwVolume);
    volume = inputvolume;
}

void Music::PlayMusicAsync(LPCSTR lpszSound) {
    // Creer un thread pour jouer la musique
    std::thread(&Music::PlayMusicWithVolumeControl, this, lpszSound).detach();
}

void Music::FadeOut(int duration) {
    // Cherche le volume actuel
    int tempvolume = volume;
    for (int downvolume = volume; downvolume >= 0; downvolume -= 1) {
        SetVolumeFadeout(downvolume);
        Sleep(duration / 100); // Temp du fade
    }
    PlaySound(NULL, NULL, 0);
    volume = tempvolume;
    SetVolumeFadeout(volume); // Arreter le son
}

void Music::StopMusic() {
    PlaySound(NULL, NULL, 0);
}