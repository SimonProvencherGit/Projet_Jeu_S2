#ifndef ENTITES_H
#define ENTITES_H

#include <windows.h>    //pour le curseur de la console et sleep()

using namespace std;

class Entite {
    protected:
    int tailleLong;
    int tailleLarg;
    char symbole;

    public:
    int posX, posY;
    bool enVie;

    Entite(int x, int y, char symb);
    virtual ~Entite() = default;
    virtual void update();
    virtual bool enCollision(int px, int py);  // retourne vrai si px et py sont egaux au x et y de l'entite
};


class Joueur : public Entite {
    private:
    int nbVies;
    int attkDmg;
    int vitesse;
    int vitesseAttk;

    public:
    Joueur(int x, int y);       //probalement autre chose a ajouter
    ~Joueur();
    void perdVie();
};

class Ennemi : public Entite {
    private:
    int type;
    int attkDmg;
    int vitesse;
    int vitesseAttk;
    int nbVies;
public:
    Ennemi(int x, int y);
    ~Ennemi();
    void perdVie();
};

class Bullet : public Entite {
    private :
    bool tirAllie;

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
