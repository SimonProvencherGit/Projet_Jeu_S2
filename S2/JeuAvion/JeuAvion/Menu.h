#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include "Interface.h"
#include "Music.h"
#include "SFX.h"

using namespace std;

class Menu {
protected:
	Interface jeux;

public:
	virtual void afficher(int choix_option) = 0;
	virtual void peser() = 0;
};

class Gerer_Menu : public Menu {
public:
	void afficher(int choix_option);
	void peser();
};

class Jouer : public Menu {
public:
	void afficher(int choix_option);
	void peser();
};

class Audio : public Menu {
private:
	Music musique;
	int volume = 50;

public:
	void afficher(int choix_option);
	void peser();
	void augmenter_volume();
	void diminuer_volume();
};

class Classement : public Menu {
private:
	vector<pair<string, int>> points;

public:
	void afficher(int choix_option);
	void peser();
	//void montrer_classement();
};

class Utilisateur : public Menu {
public:
	void afficher(int choix_option);
	void peser();
};

class Quitter : public Menu {
public:
	void afficher(int choix_option);
	void peser();
};

#endif

