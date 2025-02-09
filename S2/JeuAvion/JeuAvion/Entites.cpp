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

void Entite::perdVie()
{
    nbVies--;
    if (nbVies <= 0)
        enVie = false;

}

//******************************** classe joueur ***********************************

Joueur::Joueur(int x, int y) : Entite(x, y,'^')
{
    nbVies = 3;
    attkDmg = 1;
    vitesse = 1;
    shootCooldown = 5;
	shootTimer = 5;
	allieOuEnnemi = true;
}

Joueur::~Joueur() {}

void Joueur::update()
{
	shootTimer--;
}


//******************************** classe ennemi ***********************************

Ennemi::Ennemi(int x, int y) : Entite(x, y,'X') 
{
    type = 1;
    attkDmg = 1;
    vitesse = 1;
    shootCooldown = 50;
    nbVies = 3;
	moveTimer = 0;
	allieOuEnnemi = false;
}

Ennemi::~Ennemi() {}

BasicEnnemi::BasicEnnemi(int x, int y) : Ennemi(x, y){
    direction = rand() % 2; ; // 0 A gauche, 1 a droite
    symbole = 'W';
    shootCooldown = 40;   // 40 frames avant de tirer donc plus gros chiffre = tir plus lent
}

void BasicEnnemi::update() {

    if (moveTimer % 5 == 0) {
        if (posX <= 0 || posX >= WIDTH - 1)
            direction = 1 - direction; // Change de Direction

        posX += (direction == 0) ? -1 : 1; // Bouger a gauche ou a droite
    }

    if (moveTimer %10 == 0) 
        posY++;

	if (moveTimer >= 100)
		moveTimer = 0;

    if (posY >= HEIGHT) 
        enVie = false;
    
    moveTimer++;
}


//******************************** classe bullet ***********************************

Bullet::Bullet(int x, int y, bool isPlayerBullet) : Entite(x, y,'|')
{

}

BasicBullet::BasicBullet(int x, int y, bool isPlayerBullet) : Bullet(x, y, isPlayerBullet)
{
    symbole = '|';
    allieOuEnnemi = isPlayerBullet;
}

void BasicBullet::update()
{
    if (allieOuEnnemi)
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

