#ifndef ENTITES_H
#define ENTITES_H

#include <windows.h>    //pour le curseur de la console et sleep()

using namespace std;

class Entite {
    protected:
    int tailleLong;
    int tailleLarg;

    public:
    int posX, posY;
    int ancienX, ancienY;
    bool enVie;
    char symbole;
	int nbVies;

    Entite(int x, int y, char symb);
    virtual ~Entite() = default;
    virtual void update() = 0;
	//virtual void perdVie();
    virtual bool enCollision(int px, int py);  // retourne vrai si px et py sont egaux au x et y de l'entite
};


class Joueur : public Entite {
    private:
    int attkDmg;
    int vitesse;
    int vitesseAttk;
	

    public:
    Joueur(int x, int y);       //probalement autre chose a ajouter
    ~Joueur();
	void perdVie();
	void update() override;     //gere le deplacement du joueur
};

class Ennemi : public Entite {
    private:
    int type;
    int attkDmg;
    int vitesse;
    int vitesseAttk;
    int nbVies;
    int enemyMoveTimer;
public:
    Ennemi(int x, int y);
    ~Ennemi();
    void perdVie();
	void update() override;     //gere le deplacement de l'ennemi
};

class Bullet : public Entite {
    private :
	bool tirAllie;    //pour savoir si la balle est tiree par le joueur ou l'ennemi, détermine le sens du deplacement

    public:
    Bullet(int x, int y, bool isPlayerBullet);
    void update() override;   //gere le deplacement de la balle dependant de qui l'a tire
};

class Obstacle : public Entite{
    private :
    int nbVies;

    public:
    Obstacle(int x, int y, int longueur, int larg, int vie);     
    void update() override;    //met a jour la vie de l'obstacle
};

#endif 
