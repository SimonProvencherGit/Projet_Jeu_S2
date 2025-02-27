#include "Music.h"
#include <iostream>
#include <chrono>

Music::Music(){
}

Music::~Music() {;
    if (musicThread.joinable()) {
        musicThread.join();
    }
}

void Music::playMusic(string song) {
    thread([this, song]() {
        if (music.getStatus() == sf::SoundSource::Status::Playing) {
            FadeOut();
        }
        if (!music.openFromFile("Music\\" + song)) {
            cout << "Erreur de lecture de musique" << endl;
            return;
        }
        music.play();
        music.setLooping(true);
        }).detach();
}


void Music::stopMusic() {
    music.stop();
}

void Music::setVolume(float inputvolume)
{
    music.setVolume(inputvolume);
}

void Music::FadeOut() {
    for (int i = volume; i > 0; i=i-5) {
        music.setVolume(i);
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    music.setVolume(volume);
}
