// JeuAvion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Interface.h"
#include "Music.h"


int main() {
    SFX sfx;
    Music music;
    music.setVolume(200);
    music.playMusic("MainMenu.wav");
    printf("Press Enter To Start"); 
 
    std::cin.get();
    sfx.playSFX("Select.wav");
    music.playMusic("OceanWorld.wav");
    setConsoleSize();
    Interface game;
    game.executionJeu();

    return 0;
}