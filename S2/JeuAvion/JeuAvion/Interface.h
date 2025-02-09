#ifndef INTERFACE_H
#define INTERFACE_H

#include "Entites.h"
#include <iostream>
#include <vector>
#include <cstdlib>  //pour rand() et srand()
#include <ctime>    //pour donner point de depart aleatoire a srand() pour la generation de nombres aleatoires
#include <memory>  //pour unique_ptr 
#include <conio.h>  //pour _kbhit()
#include <string>

void setConsoleSize();

class Interface 
{
    private:
    Joueur* joueur;
    vector<unique_ptr<Entite>> listEntites;

    int score;
    bool gameOver;
    int enemySpawnTimer;

    public:
    Interface();
    void gererInput();
	void progressionDifficulte();
	void enemySpawn(int nbEnnemi, typeEnnemis enemiVoulu);
	void updateEntites();
    void updateAffichage();
    void gererCollisions();
    void enleverEntites();
    void executionJeu();
	void hideCursor();
	void showCursor();
};

#endif // INTERFACE_H