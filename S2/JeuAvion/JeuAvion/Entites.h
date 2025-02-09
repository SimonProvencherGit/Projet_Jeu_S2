#ifndef ENTITES_H
#define ENTITES_H

#include <windows.h>    //pour le curseur de la console et sleep()
//#include "Interface.h"

using namespace std;

const int WIDTH = 60;
const int HEIGHT = 50;

class Entite {
    protected:
    int tailleLong;
    int tailleLarg;

    public:
    int posX, posY;
    int shootCooldown;
	int shootTimer;
    bool enVie;
    char symbole;
	int nbVies;
    bool allieOuEnnemi;
    int moveTimer;

    Entite(int x, int y, char symb);
    virtual ~Entite() = default;
    virtual void update() = 0;
	void perdVie();
    virtual bool enCollision(int px, int py);  // retourne vrai si px et py sont egaux au x et y de l'entite
};

//-----------------------------------------------------------  classe Joueur -----------------------------------------------------------

class Joueur : public Entite {
    private:
    int attkDmg;
    int vitesse;
	

    public:
    Joueur(int x, int y);       //probalement autre chose a ajouter
    ~Joueur();
	//void perdVie();
	void update() override;     //gere le deplacement du joueur
};

//-----------------------------------------------------------  classes Ennemi -----------------------------------------------------------

class Ennemi : public Entite {
    protected:
    int type;
    int attkDmg;
    int vitesse;
    int nbVies;
    bool direction;
public:
    Ennemi(int x, int y);
    ~Ennemi();
   // void perdVie();
	virtual void update() = 0;     //gere le deplacement de l'ennemi
};

class BasicEnnemi : public Ennemi {
public:
    BasicEnnemi(int x, int y);
    void update() override;    //gere le deplacement de l'ennemi
    
};

//-----------------------------------------------------------  classes Bullet -----------------------------------------------------------

class Bullet : public Entite {
    public:
    Bullet(int x, int y, bool isPlayerBullet);
    virtual void update() = 0;   //gere le deplacement de la balle dependant de qui l'a tire
};

class BasicBullet : public Bullet {
public:
    BasicBullet(int x, int y, bool isPlayerBullet);
    void update() override;    //gere le deplacement de la balle
};

//-----------------------------------------------------------  classes Obstacle -----------------------------------------------------------

class Obstacle : public Entite{
    private :
    int nbVies;

    public:
    Obstacle(int x, int y, int longueur, int larg, int vie);     
    void update() override;    //met a jour la vie de l'obstacle
};

#endif 
