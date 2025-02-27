#include "SFX.h"
SFX::SFX() : Sound(SoundBuffer) {}

SFX::~SFX() {}

void SFX::playSFX(const std::string& sound) {
    if (!SoundBuffer.loadFromFile("SFX\\" + sound)) {
        // std::cout << "Erreur de lecture du Son" << std::endl;
        return;
    }
    Sound.setVolume(volume);
    Sound.play();
}

void SFX::stopSFX() {
    Sound.stop();
}

void SFX::setVolume(float inputvolume) {
    volume = inputvolume;
    Sound.setVolume(volume);
}

void SFX::pause() {
    Sound.pause();
}