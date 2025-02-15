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
    moveTimer = 0;
	nbVies = 1;
    bulletAllie = false;
	typeEntite = ENNEMI;      
	xJoueur = 0;
	yJoueur = 0;
	shoots = true;
	ammoType = NORMAL;
	invincible = false;
	invincibleTimer = 0;
}



bool Entite::enCollision(int px, int py) 
{
	if (px >= posX - 1 && px <= posX + largeur + 1 && py >= posY  && py < posY + hauteur)       // on fait une collision si les entites sont a la meme position (+ - 1 pour amelirer la detection)
        return 1;

    return 0;
}

void Entite::getPosJoueur(int x, int y)
{
	xJoueur = x;
	yJoueur = y;
}

void Entite::perdVie(int nbVie)
{
	for (int i = 0; i < nbVie; i++)     //joueur perd 2 vies si il entre en collision avec un ennemi   // pour peut ajouter un nb de degats a chaque ennemi
	{
		if (nbVies > 0 && enVie && !invincible)
		{
			nbVies--;
			if (nbVies == 0)
				enVie = false;
		}
	}
}

//******************************** classe joueur ***********************************

Joueur::Joueur(int x, int y) : Entite(x, y, '^', 1, 1)  //on set les valeurs par defaut pour le joueur
{
    nbVies = 10;
    attkDmg = 1;
    vitesse = 1;
    shootCooldown = 3;
	shootTimer = 0;
	bulletAllie = true;
	typeEntite = JOUEUR;
	barrelRollTimer = 0;
	barrelRoll = false;
	coolDownBarrelRoll = 0;
}


void Joueur::update()
{

	if (shootTimer > 0) 
	    shootTimer--;

	
	
	
	if (barrelRoll)					//si le joueur fait un barrel roll
	{
		barrelRoll = false;
		barrelRollTimer = 30;		//temps du barrel roll
		//invincible = true;
		symbole = '&';
		coolDownBarrelRoll = 75;		// cooldown du barrel roll
	}
	else if (invincible)		//NE MONTRE PAS LE SYMBOLE DE $ SI ININVINCILE APRES ETRE TOUCHE
	{
		invincible = false;
		invincibleTimer = 50;
		symbole = '$';

	}
	else if (barrelRollTimer <= 0 && invincibleTimer <= 0)	//si le barrel roll est fini
	{
		symbole = '^';
	}
	
	if (coolDownBarrelRoll > 0)		
		coolDownBarrelRoll--;
	
	if (barrelRollTimer > 0)
		barrelRollTimer--;

	if (invincibleTimer > 0)       //puique move timer augmente a l'infini, on le reset a 0 avant qu'il ne monte trop haut pour eviter des erreurs
		invincibleTimer--;

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
	bulletAllie = false;
	typeEntite = ENNEMI;
    typeEnnemi = BASIC;
	shoots = true;
	posRand = 0;
}

void Ennemi::update()
{
}

BasicEnnemi::BasicEnnemi(int x, int y) : Ennemi(x, y)
{
    symbole = 'W';
	nbVies = 3;
    shootCooldown = 100;   // x frames avant de tirer donc plus gros chiffre = tir plus lent
	//shootTimer = rand() % shootCooldown;   //on set le timer de tir a un nombre aleatoire entre 0 et le cooldown de tir
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
	symbole = '@';
	nbVies = 12;
	typeEnnemi = TANK;
	hauteur = 1;
	largeur = 9;
	shoots = false;
    posRand = rand() % 4;   //donne une valeur qu'on va ajouter a son y pour pas qu'ils soient tous alignes
}

void Tank::update()
{
    if (moveTimer % 5 == 0)
    {
        if(posY <= HEIGHT/3 + posRand)
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



Artilleur::Artilleur(int x, int y) : Ennemi(x, y)
{
    shoots = true;
	symbole = 'H';
    nbVies = 3;
	typeEnnemi = ARTILLEUR;
	hauteur = 2;
	largeur = 2;
    shootCooldown = 80;   // x frames avant de tirer donc plus gros chiffre = tir plus lent
	posRand = rand() % 6;   //donne une valeur qu'on va ajouter a son y pour pas qu'ils soient tous alignes
	ammoType = FRAGMENTING;
}

void Artilleur::update()
{
	if (posY <= (HEIGHT / 15) + posRand && moveTimer % 8 == 0)
		posY++;

	if (moveTimer % 50 == 0)
	{
		if (posX <= 1 || posX + largeur >= WIDTH - 1)
			direction = 1 - direction; // Change de Direction
		if (direction == 0)
			posX -= 1;
		else
			posX += 1; // Bouger a gauche ou a droite
	}

	moveTimer++;
}

Zaper::Zaper(int x, int y) : Ennemi(x, y)
{
	shoots = true;
	symbole = '*';
	nbVies = 2;
	typeEnnemi = ZAPER;
	hauteur = 2;
	largeur = 3;
	shootCooldown = 1;   
	ammoType = LASER;

}

void Zaper::update()
{
	if(posY <= HEIGHT/10 && moveTimer % 8 == 0)
		posY++;

	if (moveTimer % 25 == 0)
	{
		if (posX <= 1 || posX + largeur >= WIDTH - 1)
			direction = 1 - direction; // Change de Direction
		if (direction == 0)
			posX -= 1;
		else
			posX += 1; // Bouger a gauche ou a droite
	}

	if (moveTimer % 125 == 0)   //determine le temps on et off du laser
	{
		if (!shoots)
			shoots = true;
		else if (shoots)
			shoots = false;
	}
	
	moveTimer++;
	if (moveTimer >= 500)
		moveTimer = 0;
}


//******************************** classe bullet ***********************************

Bullet::Bullet(int x, int y, bool isPlayerBullet) : Entite(x, y,'|', 1, 1)
{
	//set des valeurs par defaut pour les bullets a etre redefinies dans les classes enfant
	nbVies = 0;
	bulletAllie = isPlayerBullet;
	typeEntite = BULLET;
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
    if (bulletAllie)
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
            if (posY >= HEIGHT+1)
                enVie = false;
        }
    }
	moveTimer++;

	if (moveTimer >= 100)
		moveTimer = 0;
}

FragmentingBullet::FragmentingBullet(int x, int y, bool isPlayerBullet) : Bullet(x, y, isPlayerBullet)
{
	symbole = 'o';
	bulletType = FRAGMENTING;
	hauteur = 1;
	largeur = 2;
	nbVies = 1;
}

void FragmentingBullet::update()
{
	if (moveTimer % 1 == 0)        //on peut aussi ajuster la vitesse des bullets ennemis
		posY++;
	
	if (posY >= HEIGHT + 1)
		enVie = false;
	
}

Laser::Laser(int x, int y, bool isPlayerBullet) : Bullet(x, y, isPlayerBullet)
{
	symbole = '~';
	bulletType = LASER;
	hauteur = HEIGHT - posY + 1;
	largeur = 1;

}


void Laser::update()
{
	if (posY >= HEIGHT - hauteur)
		enVie = false;

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

