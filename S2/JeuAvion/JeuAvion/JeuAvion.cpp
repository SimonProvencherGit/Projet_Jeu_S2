// JeuAvion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Interface.h"
#include "Music.h"
//#include "SFX.h"

int main() {
    //Music music;
    //SFX sfx;
    //music.SetVolume(3);
    //music.PlayMusicAsync("mainmenu.wav");
    printf("Press Enter To Start");
    std::cin.get();
    //sfx.PlaySFXAsync("pling.wav");
    setConsoleSize();
    Interface game;
    // music.PlayMusicAsync("ingame.wav");
    game.executionJeu();

    return 0;
}