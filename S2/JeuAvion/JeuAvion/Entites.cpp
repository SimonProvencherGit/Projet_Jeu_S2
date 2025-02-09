#include "Entites.h"

Entite::Entite(int x, int y, char symb) 
{
    posX = x;
    posY = y;

    symbole = symb;
    enVie = true;
}

void Entite::update() 
{

}

bool Entite::enCollision(int px, int py) 
{
    if(px == posX && py == posY)
        return 1;

    return 0;
}


//******************************** classe joueur ***********************************

Joueur::Joueur(int x, int y) : Entite(x, y,'^')
{
    nbVies = 3;
    attkDmg = 1;
    vitesse = 1;
    vitesseAttk = 1;
}

Joueur::~Joueur() {}

void Joueur::update()
{
}

void Joueur::perdVie() 
{
    nbVies--;
    if (nbVies <= 0) 
        enVie = false;
    
}


//******************************** classe ennemi ***********************************

Ennemi::Ennemi(int x, int y) : Entite(x, y,'X') 
{
    type = 1;
    attkDmg = 1;
    vitesse = 1;
    vitesseAttk = 1;
    nbVies = 2;
	enemyMoveTimer = 0;
}

Ennemi::~Ennemi() {}

void Ennemi::perdVie() 
{
    nbVies--;
    if (nbVies <= 0) {
        enVie = false;
    }
}

BasicEnnemi::BasicEnnemi(int x, int y) : Ennemi(x, y){
    direction = rand() % 2; ; // 0 A gauche, 1 a droite
    symbole = 'W';
}

void BasicEnnemi::update() {
    static int updateCounter = 0;
    static int shootCounter = 0;

    if (posX <= 0 || posX >= WIDTH - 1) {
        direction = 1 - direction; // Change de Direction
    }
    posX += (direction == 0) ? -1 : 1; // Bouger a gauche ou a droite

    updateCounter++;
    if (updateCounter >= 5) {
        posY++;
        updateCounter = 0;
    }

    if (posY >= HEIGHT) {
        enVie = false;
    }

    /*shootCounter++;
    if (shootCounter >= 4) {
        shoot();
        shootCounter = 0;
    }*/
}

void BasicEnnemi::shoot()
{
	//listEntites.emplace_back(std::make_unique<BasicEnnemiBullet>(posX, posY));
}

//******************************** classe bullet ***********************************

Bullet::Bullet(int x, int y, bool isPlayerBullet) : Entite(x, y,'|')
{

}

BasicBullet::BasicBullet(int x, int y, bool isPlayerBullet) : Bullet(x, y, isPlayerBullet)
{
    symbole = '|';
    tirAllie = isPlayerBullet;
}

void BasicBullet::update()
{
    if (tirAllie)
    {
        posY--;
        if (posY < 0)
            enVie = false;
    }
    else
    {
        posY++;
        if (posY >= HEIGHT)
            enVie = false;
    }
}


//******************************** classe obstacle ***********************************

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

