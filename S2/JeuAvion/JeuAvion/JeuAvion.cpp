// JeuAvion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Interface.h"
#include "Music.h"

int main() {
	Music music;
    printf("Hello World\n");
    setConsoleSize();
    Interface game;
	music.SetVolume(5);
	music.PlayMusicAsync("ingame.wav");
    game.executionJeu();

    return 0;
}