#ifndef SFX_H
#define SFX_H

#include <SFML/Audio.hpp>
#include <string>

class SFX {
public:
    SFX();
    ~SFX();
    void playSFX(const std::string& sound);
    void stopSFX();
    void setVolume(float volume);
    void pause();

private:
    float volume = 200; 
    sf::SoundBuffer SoundBuffer;
    sf::Sound Sound;
};

#endif
