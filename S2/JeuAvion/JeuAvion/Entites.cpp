#include "Entites.h"

Entite::Entite(int x, int y, char symb) 
{
	//donne des valeurs par defaut aux variables qui vont etre redefinies dans les classes enfant
    posX = x;
    posY = y;
    symbole = symb;
    enVie = true;
    collisionJoueur = false;
    shootCooldown = -1;
    shootTimer = -1;
    moveTimer = -1;
	nbVies = 1;
    allieOuEnnemi = false;
	type = ENNEMI;      
}

bool Entite::enCollision(int px, int py) 
{
	if (px >= posX - 1 && px <= posX + 1 && py == posY)       // on fait une collision si les entites sont a la meme position (+ - 1 pour amelirer la detection)
        return 1;

    return 0;
}

void Entite::perdVie()
{
    if (nbVies > 0) 
    {
        nbVies--;
        if (nbVies == 0)
            enVie = false;
    }
}

//******************************** classe joueur ***********************************

Joueur::Joueur(int x, int y) : Entite(x, y,'^')
{
    nbVies = 3;
    attkDmg = 1;
    vitesse = 1;
    shootCooldown = 3;
	shootTimer = 0;
	allieOuEnnemi = true;
	type = JOUEUR;
}

Joueur::~Joueur() {}

void Joueur::update()
{
	if (shootTimer > 0) 
	    shootTimer--;
}


//******************************** classe ennemi ***********************************

Ennemi::Ennemi(int x, int y) : Entite(x, y,'X') 
{
	//set des valeurs par defaut pour les ennemis a etre redefinies dans les classes enfant
    attkDmg = 1;
    vitesse = 1;
	direction = 0;
    shootCooldown = 50;
    shootTimer = -1;
    nbVies = 1;
	moveTimer = 0;
	allieOuEnnemi = false;
	type = ENNEMI;
    typeEnnemi = BASIC;
}

BasicEnnemi::BasicEnnemi(int x, int y) : Ennemi(x, y){
    direction = rand() % 2; ; // 0 A gauche, 1 a droite
    symbole = 'W';
	nbVies = 3;
    shootCooldown = 100;   // 100 frames avant de tirer donc plus gros chiffre = tir plus lent
	typeEnnemi = BASIC;
}

void BasicEnnemi::update() 
{
    if (moveTimer % 5 == 0)         //a toute les 5 update on peut bouger en X 
    {
        if (posX <= 0 || posX >= WIDTH - 1)
            direction = 1 - direction; // Change de Direction

        posX += (direction == 0) ? -1 : 1; // Bouger a gauche ou a droite
    }

	if (moveTimer % 8 == 0)   //a toute les 8 update on peut bouger en Y
        posY++;

	if (moveTimer >= 100)       //puique move timer augmente a l'infini, on le reset a 0 avant qu'il ne monte trop haut pour eviter des erreurs
		moveTimer = 0;

	if (posY >= HEIGHT)     //si l'ennemi atteint le bas de l'ecran is meurt
        enVie = false;
    
    moveTimer++;
}


//******************************** classe bullet ***********************************

Bullet::Bullet(int x, int y, bool isPlayerBullet) : Entite(x, y,'|')
{
	//set des valeurs par defaut pour les bullets a etre redefinies dans les classes enfant
	nbVies = 1;
	allieOuEnnemi = isPlayerBullet;
	type = BULLET;
    shootCooldown = -1;
    shootTimer = -1;
    moveTimer = 1;
	bulletType = NORMAL;
}

BasicBullet::BasicBullet(int x, int y, bool isPlayerBullet) : Bullet(x, y, isPlayerBullet)
{
    symbole = '|';
    bulletType = NORMAL;
}

void BasicBullet::update()
{
    if (allieOuEnnemi)
    {
        if (moveTimer % 1 == 0) {       //on va pouvoir changer la vitesse de la bullet dependant du modulo
            posY--;
            if (posY < 0)
                enVie = false;
        }
    }
    else    //c'est une bullet ennemi
    {
		if (moveTimer % 1 == 0) {       //on peut aussi ajuster la vitesse des bullets ennemis
            posY++;
            if (posY >= HEIGHT)
                enVie = false;
        }
    }
	moveTimer++;

	if (moveTimer >= 100)
		moveTimer = 0;
}


//******************************** classe obstacle ***********************************

Obstacle::Obstacle(int x, int y, int longueur, int larg, int vie) : Entite(x, y,'#')
{
    nbVies = vie;
}

void Obstacle::update() 
{
    if (nbVies <= 0) 
        enVie = false;
}

