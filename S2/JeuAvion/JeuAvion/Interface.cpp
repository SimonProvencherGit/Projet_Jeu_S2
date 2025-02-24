#include "Interface.h"

Interface :: Interface()
{
    //initialisation des vairalbes 
    score = 0;
    gameOver = false;
    enemySpawnTimer = 0;
	pause = false;
    posRand = 0;
	anciennePos = 0;
	explosionTimer = 0;
	enExplosion = false;
	explosionPosY = 0;
	cdExplosion = 0;
	bossSpawned = false;

    listEntites.emplace_back(make_unique<Joueur>(WIDTH / 2, HEIGHT - 1));   //ajoute le joueur a la liste d'entites
	joueur = static_cast<Joueur*>(listEntites.back().get());                //on recupere le * du joueur de la liste d'entites
}


//gere les inputs du joueur
void Interface::gererInput()
{
    if (pause == false)
    {

        if (GetAsyncKeyState(VK_LEFT) < 0 || GetAsyncKeyState('A') < 0)   //on verifie si la fleche gauche ou D est pressee
        {
            if (joueur->posX > 2)
                joueur->posX -= 2;      //on deplace le joueur de 2 vers la gauche
            else if (joueur->posX > 1)
                joueur->posX--;
        }
        if (GetAsyncKeyState(VK_RIGHT) < 0 || GetAsyncKeyState('D') < 0) {
            if (joueur->posX < WIDTH - 2)
                joueur->posX += 2;
            else if (joueur->posX < WIDTH - 1)
                joueur->posX++;
        }

        if (GetAsyncKeyState(VK_UP) < 0 || GetAsyncKeyState('W') < 0)
            if (joueur->posY > HEIGHT / 3)      //le joueur a acces au 2/3 de l'ecran
                joueur->posY--;

        if (GetAsyncKeyState(VK_DOWN) < 0 || GetAsyncKeyState('S') < 0)
            if (joueur->posY < HEIGHT)
                joueur->posY++;
        if (GetAsyncKeyState(VK_SPACE) < 0)
        {
            if (joueur->shootTimer == 0 && joueur->barrelRollTimer <= 0)    //on tire si on peut
            {
                listEntites.emplace_back(make_unique<BasicBullet>(joueur->posX + joueur->largeur / 2, joueur->posY - 1, true));
                joueur->shootTimer = joueur->shootCooldown;   //on reset le cooldown de tir du joueur pour que update puisse le faire baisser a chaque frame pour pouvoir retirer
            }
        }
        if (GetAsyncKeyState('E') < 0)
        {
            if (joueur->barrelRoll == false && joueur->coolDownBarrelRoll <= 0)
                joueur->barrelRoll = true;
        }
        if (GetAsyncKeyState('R') < 0)
        {
            if (explosionTimer == 0) 
            {

                cdExplosion = 500;      //set le cooldown de l'explosion
                enExplosion = true;
                explosionTimer = cdExplosion;
                explosionPosY = joueur->posY - 1;
            }
        }
    }

    if (explosionTimer > 0)
    {
        explosionTimer--;
        explosion();
    }

    if (GetAsyncKeyState('Q') < 0) 
        gameOver = true;
    
    if (GetAsyncKeyState('P') < 0) 
    {
		if (pause)
			pause = false;
		else if (!pause)
            pause = true;
		Sleep(200);
    }
}


void Interface::explosion()
{

    if (enExplosion)
    {
		for (auto& e : listEntites)
		{
			if (e->enVie && e->posY >= explosionPosY - 2 && e->posY <= explosionPosY + 2 && !e->isPlayer && e->typeEntite != BOSS)	//on verifie si l'entite est dans une zone d'explosion qui avance vers le haut de l'ecran
			{
				e->enVie = false;
				score += 10;
			}
		}
    }

    if(explosionPosY > 0)
	    explosionPosY--;
	else 
        enExplosion = false;
}


//fait spawn x nb d'ennemis
void Interface::enemySpawn(int nbEnnemi, typeEnnemis ennemiVoulu)
{
    posRand = (rand() % (WIDTH)) + 1;     
   
    for (int i = 0; i < nbEnnemi; i++)  //on fait spawn un nombre d'ennemis egal a nbEnnemi
    {
		//anciennePos = posRand;

        switch (ennemiVoulu)
        {
            case BASIC:
                listEntites.emplace_back(make_unique<BasicEnnemi>(posRand, 0));
			    positionSpawnRandom();    
                break;
            case RAPIDE:
                //listEntites.emplace_back(make_unique<RapideEnnemi>(posRand, 0));
                break;
            case TANK:
                listEntites.emplace_back(make_unique<Tank>(posRand, 0));
                positionSpawnRandom();
                break;
            case ARTILLEUR:
                listEntites.emplace_back(make_unique<Artilleur>(posRand, 0));
			    positionSpawnRandom();
                break;
            case DIVEBOMBER:
			    listEntites.emplace_back(make_unique<DiveBomber>(posRand, 0));
			    positionSpawnRandom();
                break;
		    case ZAPER:
			    listEntites.emplace_back(make_unique<Zaper>(posRand, 0));   
			    positionSpawnRandom();
			    break;
			case BOSS1_MAIN:
                listEntites.emplace_back(make_unique<Boss1>(WIDTH / 3, 0));
				listEntites.emplace_back(make_unique<Boss1Side>(WIDTH - WIDTH/6, 4));
                listEntites.emplace_back(make_unique<Boss1Side>(WIDTH / 6, 4));
                listEntites.emplace_back(make_unique<Boss1Side>(WIDTH / 2, 4));
				break;
        }
    }   
}

void Interface::positionSpawnRandom()       //on donne une position aleatoire a l'ennemi au spawn en evitant qu'il spawn avec une partie de lui hors de l'ecran
{
    posRand = (rand() % (WIDTH - listEntites.back()->largeur - 1)) + 1;     //on genere une position aleatoire pour l'ennemi + 1 pour ne pas etre sur la bordure
	anciennePos = posRand;
    while (posRand == anciennePos)	  //on s'assure que le nouvel ennemi n'est pas a la meme position que le dernier
        posRand = (rand() % (WIDTH - listEntites.back()->largeur - 1)) + 1;

    listEntites.back()->posX = posRand;
}


void Interface::progressionDifficulte() 
{
    enemySpawnTimer++;
	            // ******** je devrais probablement remplacer ca par des modulo du enemy spawn timer et remettre le compteur a 0 qd il est a une grande valeur  ********               
    if (score < 300)
    {
       
        if (enemySpawnTimer >= 100)          //on fait spawn une vague d'ennemis a toutes les 70 frames
        {
            enemySpawn(2, BASIC);   //on fait spawn 3 ennemis a chaque vague
			enemySpawn(1, DIVEBOMBER);
			//enemySpawn(1, TANK);
			enemySpawn(1, ARTILLEUR);
			//enemySpawn(1, ZAPER);
            
            enemySpawnTimer = 0;        //on reset le timer pour pouvoir spanw la prochaine vague d'ennemis
        }
    }
	if (score >= 300 && score < 600)
	{
		if (enemySpawnTimer >= 40)          //on fait spawn une vague d'ennemis a toutes les 60 frames
		{
			//enemySpawn(3, BASIC);   //on fait spawn 4 ennemis a chaque vague
            enemySpawn(5, DIVEBOMBER);
			enemySpawnTimer = 0;        //on reset le timer pour pouvoir spanw la prochaine vague d'ennemis
		}
	}
	if (score >= 600 && score < 1000)
	{
		if (enemySpawnTimer >= 40)          //on fait spawn une vague d'ennemis a toutes les 50 frames
		{
			enemySpawn(2, ARTILLEUR);   
			//enemySpawn(4, BASIC);   //on fait spawn 5 ennemis a chaque vague
            enemySpawn(2, DIVEBOMBER);
			enemySpawnTimer = 0;        //on reset le timer pour pouvoir spanw la prochaine vague d'ennemis
		}
	}
	if (score >= 1000 && !bossSpawned)
	{
		/*bool allEnnemiDead = true;
		for (auto& e : listEntites)
		{
			if (e->enVie = true && e->typeEntite == ENNEMI)
				allEnnemiDead = false;
		}*/
        //if (allEnnemiDead) 
        //{
            enemySpawn(1, BOSS1_MAIN);
            bossSpawned = true;
       // }
	}
}


//met a jour les entites a chaque frame
void Interface::updateEntites()
{
    vector<unique_ptr<Entite>> bufferBullets;  //on fait un buffer pour les bullets

    for (auto& e : listEntites)     //on parcourt la liste d'entites
    {
        if (e->enVie)
        {
			e->getPosJoueur(joueur->posX, joueur->posY);    //on donne la position du joueur a chaque entite, va etre utliser pour les choses a tete chercheuse etc.
			e->update();    //on met a jour l'entite

            if (e->typeEntite == ENNEMI && e->ammoType == NORMAL && e->moveTimer % e->shootCooldown == 0 && e->shoots)    //on verifie si c'est un ennemi et si sont compteur pour tirer est a 0
                bufferBullets.emplace_back(make_unique<BasicBullet>(e->posX + e->largeur / 2, e->posY + e->hauteur + 1, false));     //on cree un bullet a la position de l'ennemi qu'on met un buffer temporaire pour eviter de les ajouter a la liste d'entites pendant qu'on itere a travers d'elle  
                                                                                                                                                                                                                                            
            if (e->typeEntite == ENNEMI && e->ammoType == FRAGMENTING && e->moveTimer % e->shootCooldown == 0 && e->shoots)
                bufferBullets.emplace_back(make_unique<FragmentingBullet>(e->posX + e->largeur / 2, e->posY + e->hauteur + 1, false));
			
            if ((e->typeEntite == ENNEMI || e->typeEntite == BOSS) && e->ammoType == LASER && e->moveTimer % e->shootCooldown == 0 && e->shoots)
				bufferBullets.emplace_back(make_unique<Laser>(e->posX + e->largeur / 2, e->posY + e->hauteur + 1, false));

            if (e->typeEntite == BOSS && e->ammoType == HOMING && e->moveTimer % e->shootCooldown == 0 && e->shoots) 
            {
                bufferBullets.emplace_back(make_unique<Homing>(e->posX + e->largeur / 4, e->posY + e->hauteur + 1, false));
                bufferBullets.emplace_back(make_unique<Homing>(e->posX + e->largeur - e->largeur/ 4, e->posY + e->hauteur + 1, false));
                bufferBullets.emplace_back(make_unique<Homing>(e->posX + e->largeur / 2, e->posY + e->hauteur + 1, false));
            }

            if (e->typeEntite == BOSS && e->ammoType == HOMING)
            {
				e->invincible = false;

				for (auto& e2 : listEntites)
				{
                    if (e2->enVie == true && e2->typeEntite == BOSS && e2->ammoType == LASER)
                    {
						e->invincible = true;
                    }
				}
            }
        }
    }
	for (auto& bullet : bufferBullets) 
		listEntites.push_back(move(bullet));	//on ajoute les bullets du buffer a la liste d'entites
}


//gere les collisions entre les entites
void Interface::gererCollisions()
{
    vector<unique_ptr<Entite>> bufferBullets;  //on fait un buffer pour les bullets car on ne peut pas ajouter a la liste entite pendant qu`on itere a travers

    for (auto& e : listEntites)
    {
        //if (e->enVie)
        //{
            if (e->enCollision(joueur->posX, joueur->posY) && joueur->invincibleTimer <= 0 && joueur->barrelRollTimer <= 0 && !e->isPlayer)     //on verifie si un entite entre en collision avec le joueur et verifie que e n'est pas joueur
            {
                if (e->typeEntite == ENNEMI && e->collisionJoueur == false)
                {
					joueur->perdVie(2);	 //le joueur perd 2 vies si il entre en collision avec un ennemi
					joueur->invincible = true;     //le joueur est invincible pour un court moment apres

                    if (!joueur->enVie)
                        gameOver = true;
                    
                    e->collisionJoueur = true;
                }
                else if (e->typeEntite == BULLET && e->collisionJoueur == false  && !e->bulletAllie)     //si le joueur entre en collision avec une bullet ennemi sans etre en barrel roll il perd une vie
                {
					joueur->perdVie(1);    //le joueur perd 1 vie si il entre en collision avec une bullet ennemi   
					joueur->invincible = true;     //le joueur est invincible pour un court moment apres

                    if (!joueur->enVie)
                        gameOver = true;

                    e->collisionJoueur = true;
                }
            }
			//partie ou on gere les collision avec les bullets alliees
			else if (e->typeEntite == BULLET && e->bulletAllie)  //on verifie si c'est un bullet allie tire par le joueur
            {
				for (auto& e2 : listEntites)	//on parcourt la liste d'entites pour voir si la bullet entre en collision avec un ennemi
                {
                    if (e2->enVie)
                    {
                        if (e2->enVie && e2->enCollision(e->posX, e->posY) && e2->symbole != e->symbole)       // si qqlch entre en collision avec la bullet allie et le e->symbole est pour pas que la bullet entre en collision avec elle meme 
                        {
                             if (e2->ammoType == FRAGMENTING && e2->typeEntite == BULLET && !e2->bulletAllie)      //si c'est un fragmenting bullet d'unennemi
                                for (int i = -1; i < 2; i++)	//commence a -1 pour que le premier fragment commence a la gauche de la bullet
                                    bufferBullets.emplace_back(make_unique<BasicBullet>(e2->posX+i, e2->posY + 1, false));

                            e2->perdVie(1);
                            e->enVie = false;

                            if (!e2->enVie && e2->typeEntite != BULLET)
                                score += 10;
                        }
                    }
                }
            }
            else 
                e->collisionJoueur = false;
        //}
    }
    for (auto& bullet : bufferBullets)
        listEntites.push_back(move(bullet));	//on ajoute les bullets du buffer a la liste d'entites
}


//met a jour l'affichage de la console 
void Interface::updateAffichage()
{
	wchar_t buffer[HEIGHT + 4][WIDTH + 2];  // +3 pour les bordures et le score, +2 pour les bordures
    wchar_t progressExplosion[12];	//buffer pour afficher progression de l'explosion
    wchar_t progressBarrelRoll[12];	//buffer pour afficher progression du barrel roll
	float nbSymboleExplosion = 0;  //pour le nombre de symboles a afficher dans la progression de l'explosion
    float nbSymboleBarrelRoll = 0;  
	wstring texte;  //on cree un string pour afficher le score, le nombre de vies et autres textes
    

    // Remplir le tampon avec des espaces (effacer l'écran)
    for (int y = 0; y < HEIGHT + 4; y++) {
        for (int x = 0; x < WIDTH + 2; x++) {
            buffer[y][x] = L' ';
        }
    }

	//rempli le buffer progressexplosion avec des espaces
	for (int i = 0; i < 12; i++)
		progressExplosion[i] = L' ';

	//rempli le buffer progressBarrelRoll avec des espaces
	for (int i = 0; i < 12; i++)
		progressBarrelRoll[i] = L' ';


    // Mettre à jour les bordures
	for (int x = 0; x < WIDTH + 2; x++)     //on met les bordures du haut et du bas
    {
        buffer[0][x] = L'#';
        buffer[HEIGHT + 1][x] = L'#';
    }
	for (int y = 1; y <= HEIGHT; y++)   //on met les bordures de gauche et de droite, on commence a 1 et finit a HEIGHT pour pas ecraser les bordures du haut et du bas
    {
        buffer[y][0] = L'#';
        buffer[y][WIDTH + 1] = L'#';
    }

    // Mettre à jour les entités vivantes
    for (auto& e : listEntites) 
    {
        if (e->enVie)
        {
			for (int y = 0; y < e->hauteur; y++)        //on parcourt la hauteur de l'entite
            {
				for (int x = 0; x < e->largeur; x++)    //on parcourt la largeur de l'entite
                {
					buffer[e->posY + y][e->posX + x] = e->symbole;  //on met a jour la position de l'entite dans le buffer
                }
            }
        }
    }

    //Affiche progression pour l'explosion
	progressExplosion[0] = L'[';
	progressExplosion[11] = L']';

    if (explosionTimer == 0)
        nbSymboleExplosion = 10;
    else
        nbSymboleExplosion = ((float)(cdExplosion - explosionTimer) / (float)cdExplosion) * 10;

	for (int i = 0; i < (int)nbSymboleExplosion; i++)
        progressExplosion[i+1] = L'#';

	texte = L"Explosion: ";
	for (int i = 0; i < texte.size(); i++)   //on ajoute le score au buffer
		buffer[HEIGHT + 2][i] = texte[i];
   
    for (int i = 0; i < 12; i++)
        buffer[HEIGHT + 2][i + texte.size()] = progressExplosion[i];

	//Affiche progression pour le barrel roll
	progressBarrelRoll[0] = L'[';
	progressBarrelRoll[11] = L']';
	int posCurseur = 12 + texte.size() + 5;

	if (joueur->coolDownBarrelRoll == 0)
        nbSymboleBarrelRoll = 10;
    else 
    {
        nbSymboleBarrelRoll = ((float)(CD_BARRELROLL - joueur->coolDownBarrelRoll) / (float)CD_BARRELROLL) * 10;
		if (nbSymboleBarrelRoll > 10)
			nbSymboleBarrelRoll = 10;
    }

	for (int i = 0; i < (int)nbSymboleBarrelRoll; i++)
		progressBarrelRoll[i + 1] = L'#';

	texte = L"Barrel Roll: ";
	for (int i = 0; i < texte.size(); i++)   //on ajoute le score au buffer
		buffer[HEIGHT + 2][i + posCurseur] = texte[i];

	for (int i = 0; i < 12; i++)
		buffer[HEIGHT + 2][i + texte.size() + posCurseur] = progressBarrelRoll[i];

    
    // Afficher le score
    if (gameOver)
        texte = L"Game Over!  Score: " + to_wstring(score);
    else
        texte = L"Score: " + to_wstring(score) + L"   Lives: " + to_wstring(joueur->nbVies);

	for (size_t i = 0; i < texte.size(); i++)   //on ajoute le score au buffer
        buffer[HEIGHT + 3][i] = texte[i];
    

	// on affiche chaque ligne du buffer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < HEIGHT + 4; y++) 
    {
        COORD bufferCoord = { 0, static_cast<SHORT>(y) };
        DWORD charsWritten;
        
		WriteConsoleOutputCharacterW(hConsole, buffer[y], WIDTH + 2, bufferCoord, &charsWritten);       //on ecrit chaque ligne du buffer
    }
}


//enleve les entites mortes de la liste d'entites
void Interface :: enleverEntites()
{
	for (int i = 0; i < listEntites.size(); i++)
	{
		if (!listEntites[i]->enVie)
		{
			listEntites.erase(listEntites.begin() + i);
			i--;
		}
	}
}


//execution du jeu
void Interface :: executionJeu()
{
    hideCursor();
    while(!gameOver)
    {
            gererInput();
     
            while (pause == true)
            {
                gererInput();   //sert a revenir au jeu si on a fait pause
                Sleep(10);
            }
		    progressionDifficulte();
		    updateEntites();
            gererCollisions();
            enleverEntites();
            updateAffichage();
            Sleep(25);

    }
	Sleep(1500);
    showCursor();
}


//Set la taille de la console
void setConsoleSize() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        cerr << "Error getting console handle" << endl;
        return;
    }
    //COORD bufferSize = { WIDTH, HEIGHT };
    //SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set la taille de la console
    SMALL_RECT windowSize = { 0, 0, WIDTH+3, HEIGHT+3};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

//cache le curseur de la console
void Interface::hideCursor() 
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//affiche le curseur de la console
void Interface::showCursor() 
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}