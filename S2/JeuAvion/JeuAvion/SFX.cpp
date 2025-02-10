#include "SFX.h"
#include <vector>
#include <chrono>
#include <thread>

SFX::SFX() {
    // Initialize a pool of HWAVEOUT devices
    for (int i = 0; i < 10; ++i) {
        HWAVEOUT hWaveOut;
        WAVEFORMATEX wfx;
        wfx.nSamplesPerSec = 44100; // Sample rate
        wfx.wBitsPerSample = 16;    // Sample size
        wfx.nChannels = 2;          // Number of channels
        wfx.cbSize = 0;             // Size of the structure
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) / 8;
        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

        MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
        if (result == MMSYSERR_NOERROR) {
            waveOutPool.push_back(hWaveOut);
        }
        else {
            std::cerr << "Error opening waveform-audio output device." << std::endl;
        }
    }
}

SFX::~SFX() {
    for (HWAVEOUT hWaveOut : waveOutPool) {
        waveOutClose(hWaveOut);
    }
}

HWAVEOUT SFX::getAvailableWaveOut() {
    std::lock_guard<std::mutex> lock(poolMutex);
    if (!waveOutPool.empty()) {
        HWAVEOUT hWaveOut = waveOutPool.back();
        waveOutPool.pop_back();
        return hWaveOut;
    }
    return NULL;
}

void SFX::releaseWaveOut(HWAVEOUT hWaveOut) {
    std::lock_guard<std::mutex> lock(poolMutex);
    waveOutPool.push_back(hWaveOut);
}

void SFX::PlaySFXAsync(LPCSTR lpszSound) {
    std::thread([this, lpszSound]() {
        HWAVEOUT hWaveOut = getAvailableWaveOut();
        if (!hWaveOut) {
            std::cerr << "No available waveform-audio output device." << std::endl;
            return;
        }

        // Load the sound data
        HMMIO hMmio = mmioOpenA(const_cast<LPSTR>(lpszSound), NULL, MMIO_READ | MMIO_ALLOCBUF);
        if (!hMmio) {
            std::cerr << "Error opening sound file." << std::endl;
            releaseWaveOut(hWaveOut);
            return;
        }

        MMCKINFO ckIn;
        MMCKINFO ckInfo;
        PCMWAVEFORMAT pcmWaveFormat;

        // Locate a 'RIFF' chunk with a 'WAVE' form type
        ckIn.fccType = mmioFOURCC('W', 'A', 'V', 'E');
        if (mmioDescend(hMmio, &ckIn, NULL, MMIO_FINDRIFF)) {
            std::cerr << "This is not a WAVE file." << std::endl;
            mmioClose(hMmio, 0);
            releaseWaveOut(hWaveOut);
            return;
        }

        // Locate the 'fmt ' chunk
        ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
        if (mmioDescend(hMmio, &ckInfo, &ckIn, MMIO_FINDCHUNK)) {
            std::cerr << "Error finding 'fmt ' chunk." << std::endl;
            mmioClose(hMmio, 0);
            releaseWaveOut(hWaveOut);
            return;
        }

        // Read the 'fmt ' chunk
        if (mmioRead(hMmio, reinterpret_cast<HPSTR>(&pcmWaveFormat), sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat)) {
            std::cerr << "Error reading 'fmt ' chunk." << std::endl;
            mmioClose(hMmio, 0);
            releaseWaveOut(hWaveOut);
            return;
        }

        // Ascend out of the 'fmt ' chunk
        mmioAscend(hMmio, &ckInfo, 0);

        // Locate the 'data' chunk
        ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
        if (mmioDescend(hMmio, &ckInfo, &ckIn, MMIO_FINDCHUNK)) {
            std::cerr << "Error finding 'data' chunk." << std::endl;
            mmioClose(hMmio, 0);
            releaseWaveOut(hWaveOut);
            return;
        }

        // Allocate memory for the sound data
        DWORD dataSize = ckInfo.cksize;
        BYTE* data = new BYTE[dataSize];
        if (mmioRead(hMmio, reinterpret_cast<HPSTR>(data), dataSize) != dataSize) {
            std::cerr << "Error reading sound data." << std::endl;
            delete[] data;
            mmioClose(hMmio, 0);
            releaseWaveOut(hWaveOut);
            return;
        }

        // Close the file
        mmioClose(hMmio, 0);

        // Prepare the header
        WAVEHDR waveHdr;
        waveHdr.lpData = reinterpret_cast<LPSTR>(data);
        waveHdr.dwBufferLength = dataSize;
        waveHdr.dwFlags = 0;
        waveHdr.dwLoops = 0;

        if (waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
            std::cerr << "Error preparing header." << std::endl;
            delete[] data;
            releaseWaveOut(hWaveOut);
            return;
        }

        // Play the sound
        if (waveOutWrite(hWaveOut, &waveHdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
            std::cerr << "Error writing to device." << std::endl;
            waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(WAVEHDR));
            delete[] data;
            releaseWaveOut(hWaveOut);
            return;
        }

        // Wait for the sound to finish playing
        while (!(waveHdr.dwFlags & WHDR_DONE)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Clean up
        waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(WAVEHDR));
        delete[] data;
        releaseWaveOut(hWaveOut);
        }).detach();
}
