#ifndef ENTITES_H
#define ENTITES_H

#include <windows.h>    //pour le curseur de la console et sleep()
//#include "Interface.h"
#include "SFX.h"

using namespace std;

//definit la taille du jeu
const int WIDTH = 100;
const int HEIGHT = 37;
const int CD_BARRELROLL = 75;

enum typeEntites { JOUEUR, ENNEMI, OBSTACLE, BULLET, BOSS};
enum typeEnnemis { BASIC, TANK, ARTILLEUR, DIVEBOMBER, ZAPER, AIMBOT, BOSS1_MAIN, BOSS1_SIDE };
enum typeBullets { NORMAL, LASER, MULTIPLE, HOMING, BOMB, FRAGMENTING };

class Entite
{

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
    bool isPlayer;

    Entite(int x, int y, char symb, int longueurEntite, int largeurEntite);
    virtual void update() = 0;
    void perdVie(int nbVie);
    virtual bool enCollision(int px, int py);  // retourne vrai si px et py sont egaux au x et y de l'entite
    virtual void getPosJoueur(int x, int y);
	virtual typeEnnemis getTypeEnnemi();
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
    int attkDmg;
    int vitesse;
    bool direction;
    int posRand;
    typeEnnemis typeEnnemi;

public:
    Ennemi(int x, int y);
    virtual void update() = 0;     //gere le deplacement de l'ennemi
    typeEnnemis getTypeEnnemi() override;
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

class Aimbot : public Ennemi
{
public:
    Aimbot(int x, int y);
    void update();
};

class Boss1 : public Ennemi
{
public:
    Boss1(int x, int y);
    void update();    //gere le deplacement de l'ennemi
};

class Boss1Side : public Ennemi
{
public:
    bool shootTiming;
    bool firstEntry;
    Boss1Side(int x, int y);
    void update();    //gere le deplacement de l'ennemi
};

//-----------------------------------------------------------  classes Bullet -----------------------------------------------------------

class Bullet : public Entite
{
public:

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

class Homing : public Bullet
{
public:
    Homing(int x, int y, bool isPlayerBullet);
    void update();    //gere le deplacement de la balle
};


//-----------------------------------------------------------  classes Obstacle -----------------------------------------------------------

class Obstacle : public Entite {
private:
    //int nbVies;

public:
    Obstacle(int x, int y, int longueur, int larg, int vie);
    void update();    //met a jour la vie de l'obstacle
};

#endif 
