#ifndef ENTITES_H
#define ENTITES_H

#include <windows.h>    //pour le curseur de la console et sleep()
//#include "Interface.h"
#include "SFX.h"

using namespace std;

//definit la taille du jeu
const int WIDTH = 60;
const int HEIGHT = 50;
enum typeEntites{JOUEUR, ENNEMI, OBSTACLE, BULLET};
enum typeEnnemis {BASIC, RAPIDE, TANK, ARTILLEUR, DIVEBOMBER, BOSS};
enum typeBullets {NORMAL, LASER, MULTIPLE, HOMING, BOMB};

class Entite 
{
    protected:
    //int tailleLong;
    //int tailleLarg;

    public:
    int posX, posY;
	int xJoueur, yJoueur;
	int largeur, hauteur;
    int shootCooldown;
	int shootTimer;
    bool enVie;
    char symbole;
	int nbVies;
    bool allieOuEnnemi;
    int moveTimer;
    bool collisionJoueur;
	typeEntites type;

    Entite(int x, int y, char symb, int longueurEntite, int largeurEntite);
    virtual void update() = 0;
	void perdVie();
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

class RapideEnnemi : public Ennemi
{
    public:
	RapideEnnemi(int x, int y);
	void update();    //gere le deplacement de l'ennemi
};

class DiveBomber : public Ennemi
{
    public:
	DiveBomber(int x, int y);
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

//-----------------------------------------------------------  classes Obstacle -----------------------------------------------------------

class Obstacle : public Entite{
    private :
    //int nbVies;

    public:
    Obstacle(int x, int y, int longueur, int larg, int vie);     
    void update();    //met a jour la vie de l'obstacle
};

#endif 
