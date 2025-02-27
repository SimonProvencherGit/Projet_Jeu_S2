#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <SFML/Audio.hpp>
#include <thread>
using namespace std;
class Music {
public:
    Music();
    ~Music();
    void playMusic(std::string song);
    void stopMusic();
    void setVolume(float volume);

private:
    void FadeOut();
    sf::Music music;
    int volume = 200;
    std::thread musicThread;
};

#endif // MUSIC_H
