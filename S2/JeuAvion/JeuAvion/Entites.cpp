#include "Entites.h"
SFX sfxx;        //explication a mettre ici

Entite::Entite(int x, int y, char symb, int largeurEntite, int hauteurEntite)
{
	//donne des valeurs par defaut aux variables qui vont etre redefinies dans les classes enfant
    posX = x;
    posY = y;
	hauteur = hauteurEntite;
	largeur = largeurEntite;
    symbole = symb;
    enVie = true;
    collisionJoueur = false;
    shootCooldown = -1;
    shootTimer = -1;
    moveTimer = -1;
	nbVies = 1;
    allieOuEnnemi = false;
	type = ENNEMI;      
	xJoueur = 0;
	yJoueur = 0;
}



bool Entite::enCollision(int px, int py) 
{
	if (px >= posX - 1 && px <= posX + largeur + 1 && py >= posY && py < posY + hauteur)       // on fait une collision si les entites sont a la meme position (+ - 1 pour amelirer la detection)
        return 1;

    return 0;
}

void Entite::getPosJoueur(int x, int y)
{
	xJoueur = x;
	yJoueur = y;
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

Joueur::Joueur(int x, int y) : Entite(x, y, '^', 1, 1)  //on set les valeurs par defaut pour le joueur
{
    nbVies = 3;
    attkDmg = 1;
    vitesse = 1;
    shootCooldown = 3;
	shootTimer = 0;
	allieOuEnnemi = true;
	type = JOUEUR;
}


void Joueur::update()
{
	if (shootTimer > 0) 
	    shootTimer--;
}


//******************************** classe ennemi ***********************************

Ennemi::Ennemi(int x, int y) : Entite(x, y, 'X', 1, 1) 
{
	//set des valeurs par defaut pour les ennemis a etre redefinies dans les classes enfant
    attkDmg = 1;
    vitesse = 1;
    direction = rand() % 2; ; // 0 A gauche, 1 a droite
    shootCooldown = 50;
    shootTimer = -1;
    nbVies = 1;
	moveTimer = 0;
	allieOuEnnemi = false;
	type = ENNEMI;
    typeEnnemi = BASIC;
	shoots = true;
}

void Ennemi::update()
{
}

BasicEnnemi::BasicEnnemi(int x, int y) : Ennemi(x, y)
{
    symbole = 'W';
	nbVies = 3;
    shootCooldown = 100;   // x frames avant de tirer donc plus gros chiffre = tir plus lent
	shootTimer = rand() % shootCooldown;   //on set le timer de tir a un nombre aleatoire entre 0 et le cooldown de tir
    typeEnnemi = BASIC;
    hauteur = 2;
	largeur = 4;
	moveTimer = rand() % shootCooldown;   //on set le timer de mouvement a un nombre aleatoire entre 0 et le cooldown de tir pour que les ennemis tirent a des moments differents
}

void BasicEnnemi::update()
{
    if (moveTimer % 5 == 0)         //a toute les 5 update on peut bouger en X 
    {
        if (posX <= 1 || posX + largeur >= WIDTH-1)
            direction = 1 - direction; // Change de Direction

        if (direction == 0)
            posX -= 1;
        else
            posX += 1; // Bouger a gauche ou a droite
    }

	if (moveTimer % 8 == 0)   //a toute les 8 update on peut bouger en Y
        posY++;

	if (moveTimer >= 100)       //puique move timer augmente a l'infini, on le reset a 0 avant qu'il ne monte trop haut pour eviter des erreurs
		moveTimer = 0;

	if (posY >= HEIGHT)     //si l'ennemi atteint le bas de l'ecran is meurt
        enVie = false;
    
    moveTimer++;
}


DiveBomber::DiveBomber(int x, int y) : Ennemi(x, y)
{
    symbole = 'V';
    nbVies = 3;
    typeEnnemi = DIVEBOMBER;
    hauteur = 4;
    largeur = 2;
	shoots = false;
}

//le diveBomber est kamikaze qui va directement vers le joueur
void DiveBomber::update()
{
	if (moveTimer % 2 == 0)         //on peut ajuster la vitesse en x du diveBomber
	{
		if (posX < xJoueur)
			posX++;
		else if (posX > xJoueur)
			posX--;
	}
	if (moveTimer % 1 == 0)         //on peut ajuster la vitesse du diveBomber
	{
		posY++;
	}
	if (moveTimer >= 100)       //puique move timer augmente a l'infini, on le reset a 0 avant qu'il ne monte trop haut pour eviter des erreurs
		moveTimer = 0;
	if (posY >= HEIGHT)     //si l'ennemi atteint le bas de l'ecran is meurt
		enVie = false;
	moveTimer++;
}

Tank::Tank(int x, int y) : Ennemi(x, y)
{
	symbole = '=';
	nbVies = 15;
	typeEnnemi = TANK;
	hauteur = 1;
	largeur = 10;
	shoots = false;
}

void Tank::update()
{
    if (moveTimer % 5 == 0)
    {
        if(posY <= HEIGHT/3)
		    posY++;
    }
	if (moveTimer % 50 == 0)
	{
		if (posX <= 1 || posX + largeur >= WIDTH - 1)
			direction = 1 - direction; // Change de Direction
        if (direction == 0) 
            posX -= 1; 
        else 
            posX += 1; // Bouger a gauche ou a droite
	}
    if (moveTimer >= 100)       //puique move timer augmente a l'infini, on le reset a 0 avant qu'il ne monte trop haut pour eviter des erreurs
        moveTimer = 0;
    moveTimer++;
}


//******************************** classe bullet ***********************************

Bullet::Bullet(int x, int y, bool isPlayerBullet) : Entite(x, y,'|', 1, 1)
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
	hauteur = 1;
	largeur = 1;
    sfxx.PlaySFXAsync("basicbullet.wav");
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

Obstacle::Obstacle(int x, int y, int longueur, int larg, int vie) : Entite(x, y,'#', 3, 1)
{
    nbVies = vie;
}

void Obstacle::update() 
{
    if (nbVies <= 0) 
        enVie = false;
}

