#ifndef ENTITES_H
#define ENTITES_H

#include <windows.h>    //pour le curseur de la console et sleep()
//#include "Interface.h"
#include "SFX.h"

using namespace std;

//definit la taille du jeu
const int WIDTH = 100;
const int HEIGHT = 40;
enum typeEntites{JOUEUR, ENNEMI, OBSTACLE, BULLET};
enum typeEnnemis {BASIC, RAPIDE, TANK, ARTILLEUR, DIVEBOMBER, ZAPER, BOSS};
enum typeBullets {NORMAL, LASER, MULTIPLE, HOMING, BOMB, FRAGMENTING};

class Entite 
{
    protected:

    public:
    int posX, posY;
	int xJoueur, yJoueur;
	int largeur, hauteur;
    int shootCooldown;
	int shootTimer;
    bool enVie;
    char symbole;
	int nbVies;
    bool bulletAllie;
    int moveTimer;
    bool collisionJoueur;
	typeEntites typeEntite;
	typeBullets ammoType;
    bool shoots;
    bool invincible;
	int invincibleTimer;

    Entite(int x, int y, char symb, int longueurEntite, int largeurEntite);
    virtual void update() = 0;
	void perdVie(int nbVie);
    virtual bool enCollision(int px, int py);  // retourne vrai si px et py sont egaux au x et y de l'entite
    virtual void getPosJoueur(int x, int y);
};

//-----------------------------------------------------------  classe Joueur -----------------------------------------------------------

class Joueur : public Entite 
{
    private:
    int attkDmg;
    int vitesse;

    public:
	bool barrelRoll;  
    int barrelRollTimer;
    int coolDownBarrelRoll;

    Joueur(int x, int y);       //probalement autre chose a ajouter
	void update();     //gere le deplacement du joueur
};

//-----------------------------------------------------------  classes Ennemi -----------------------------------------------------------

class Ennemi : public Entite 
{
    protected:
    typeEnnemis typeEnnemi;
    int attkDmg;
    int vitesse;
    bool direction;
	int posRand;

    public:
    Ennemi(int x, int y);
	virtual void update() = 0;     //gere le deplacement de l'ennemi
};

class BasicEnnemi : public Ennemi 
{
    public:
    BasicEnnemi(int x, int y);
    void update();    //gere le deplacement de l'ennemi
    
};

class Tank : public Ennemi
{
    public:
	Tank(int x, int y);
	void update();    //gere le deplacement de l'ennemi
};

class DiveBomber : public Ennemi
{
    public:
	DiveBomber(int x, int y);
	void update();    //gere le deplacement de l'ennemi
};

class Artilleur : public Ennemi
{
    public:
	Artilleur(int x, int y);
	void update();    //gere le deplacement de l'ennemi
};

class Zaper : public Ennemi
{
    public:
	Zaper(int x, int y);
	void update();    //gere le deplacement de l'ennemi
};

//-----------------------------------------------------------  classes Bullet -----------------------------------------------------------

class Bullet : public Entite 
{
    public:
	typeBullets bulletType;

    Bullet(int x, int y, bool isPlayerBullet);
	virtual void update() = 0;   //gere le deplacement de la balle dependant de qui l'a tire on donne la pos x, y du joueur pour les bullets a tete chercheuse
};

class BasicBullet : public Bullet 
{
    public:
    BasicBullet(int x, int y, bool isPlayerBullet);
    void update() override;    //gere le deplacement de la balle
};

class FragmentingBullet : public Bullet
{
public:
	FragmentingBullet(int x, int y, bool isPlayerBullet);
	void update();    //gere le deplacement de la balle
};

class Laser : public Bullet
{
public:
	Laser(int x, int y, bool isPlayerBullet);
	void update();    //gere le deplacement de la balle
};

//-----------------------------------------------------------  classes Obstacle -----------------------------------------------------------

class Obstacle : public Entite{
    private :
    //int nbVies;

    public:
    Obstacle(int x, int y, int longueur, int larg, int vie);     
    void update();    //met a jour la vie de l'obstacle
};

#endif 
