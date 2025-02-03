#ifndef INTERFACE_H
#define INTERFACE_H

#include "Entites.h"
#include <iostream>
#include <vector>
#include <cstdlib>  //pour rand() et srand()
#include <ctime>    //pour donner point de depart aleatoire a srand() pour la generation de nombres aleatoires
#include <memory>  //pour unique_ptr 
#include <conio.h>  //pour _kbhit()

const int WIDTH = 50;
const int HEIGHT = 20;

class Interface 
{
    private:
    unique_ptr<Joueur> joueur;
    vector<unique_ptr<Entite>> listEntites;
    vector<vector<Entite*>> affichage;
    int score;
    bool gameOver;
    int enemyDirection;
    int enemyMoveTimer;

    public:
    Interface();
    ~Interface();
    void gererInput();
    void updateAffichage();
    void updateEntites();
    void gererCollisions();
    void enleverEntites();
    void executionJeu();

};

#endif // INTERFACE_H