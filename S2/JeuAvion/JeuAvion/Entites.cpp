#include "Entites.h"

Entite::Entite(int x, int y, char symb) 
{
    posX = x;
    posY = y;
    symbole = symb;
    enVie = true;
}

void Entite::update() {}

bool Entite::enCollision(int px, int py) 
{
    if(px == posX && py == posY)
        return 1;

    return 0;
}


//******************************** classe joueur ***********************************//

Joueur::Joueur(int x, int y) : Entite(x, y,'^')
{
    nbVies = 3;
    attkDmg = 1;
    vitesse = 1;
    vitesseAttk = 1;
}

Joueur::~Joueur() {}

void Joueur::perdVie() 
{
    nbVies--;
    if (nbVies <= 0) 
        enVie = false;
    
}


//******************************** classe ennemi ***********************************//

Ennemi::Ennemi(int x, int y) : Entite(x, y,'X') 
{
    type = 1;
    attkDmg = 1;
    vitesse = 1;
    vitesseAttk = 1;
    nbVies = 2;
}

Ennemi::~Ennemi() {}

void Ennemi::perdVie() 
{
    nbVies--;
    if (nbVies <= 0) {
        enVie = false;
    }
}


//******************************** classe bullet ***********************************//

Bullet::Bullet(int x, int y, bool isPlayerBullet) : Entite(x, y,'|')
{

}

void Bullet::update() 
{
    if (tirAllie) 
        posY--;
    else 
        posY++;
    
}


//******************************** classe obstacle ***********************************//

Obstacle::Obstacle(int x, int y, int longueur, int larg, int vie) : Entite(x, y,'#')
{
    tailleLong = longueur;
    tailleLarg = larg;
    nbVies = vie;
}

void Obstacle::update() 
{
    if (nbVies <= 0) 
        enVie = false;
}




