#include "Interface.h"

Interface :: Interface()
{
    //initialisation des vairalbes 
    score = 0;
    gameOver = false;
    enemySpawnTimer = 0;

    listEntites.emplace_back(make_unique<Joueur>(WIDTH / 2, HEIGHT - 1));   //ajoute le joueur a la liste d'entites
	joueur = static_cast<Joueur*>(listEntites.back().get());                //on recupere le * du joueur de la liste d'entites
}


//gere les inputs du joueur
void Interface :: gererInput()
{
	if (GetAsyncKeyState(VK_LEFT) < 0 || GetAsyncKeyState('A') < 0)   //on verifie si la fleche gauche ou D est pressee
        if (joueur->posX > 0) 
            joueur->posX--;
    
    if (GetAsyncKeyState(VK_RIGHT) < 0 || GetAsyncKeyState('D') < 0)
        if (joueur->posX < WIDTH - 1) 
            joueur->posX++;
    
    if (GetAsyncKeyState(VK_UP) < 0 || GetAsyncKeyState('W') < 0)
        if (joueur->posY > HEIGHT / 3)      //le joueur a acces au 2/3 de l'ecran
            joueur->posY--;
   
    if (GetAsyncKeyState(VK_DOWN) < 0 || GetAsyncKeyState('S') < 0)
        if (joueur->posY < HEIGHT - 1) 
            joueur->posY++;
    if (GetAsyncKeyState(VK_SPACE) < 0) 
    {
		if (joueur->shootTimer == 0)    //on tire si on peut
        {
            listEntites.emplace_back(make_unique<BasicBullet>(joueur->posX + joueur->largeur/2, joueur->posY - 1, true));
			joueur->shootTimer = joueur->shootCooldown;   //on reset le cooldown de tir du joueur pour que update puisse le faire baisser a chaque frame pour pouvoir retirer
        }
    }
    if (GetAsyncKeyState('Q') < 0) {
        gameOver = true;
    }
}


//fait spawn x nb d'ennemis
void Interface::enemySpawn(int nbEnnemi, typeEnnemis ennemiVoulu)
{
    int posRand = (rand() % (WIDTH)) + 1;     //on genere une position aleatoire pour l'ennemi + 1 pour ne pas etre sur la bordure
	int anciennePos;	//on garde en memoire la position de l'ennemi precedent pour eviter de le spawn a la meme position
	
   
    for (int i = 0; i < nbEnnemi; i++)  //on fait spawn un nombre d'ennemis egal a nbEnnemi
    {
		anciennePos = posRand;

        switch (ennemiVoulu)
        {
        case BASIC:
            listEntites.emplace_back(make_unique<BasicEnnemi>(posRand, 0));
			posRand = (rand() % (WIDTH - listEntites.back()->largeur -1)) + 1;     //on genere une position aleatoire pour l'ennemi + 1 pour ne pas etre sur la bordure
            while (posRand == anciennePos)	  //on s'assure que le nouvel ennemi n'est pas a la meme position que le dernier
            {
				posRand = (rand() % (WIDTH - listEntites.back()->largeur - 1)) + 1;
            }
            break;
        case RAPIDE:
            //listEntites.emplace_back(make_unique<RapideEnnemi>(posRand, 0));
            break;
        case TANK:
            //listEntites.emplace_back(make_unique<TankEnnemi>(posRand, 0));
            break;
        case ARTILLEUR:
            //listEntites.emplace_back(make_unique<ArtilleurEnnemi>(posRand, 0));
            break;
        case DIVEBOMBER:
            //listEntites.emplace_back(make_unique<DiveBomberEnnemi>(posRand, 0));
            break;
        }
    }

    /*
	for (int i = 0; i < nbEnnemi; i++)  //on fait spawn un nombre d'ennemis egal a nbEnnemi
    {
        anciennePos = posRand;
        posRand = 1 + rand() % (WIDTH-1);
		while (posRand == anciennePos)	  //on s'assure que le nouvel ennemi n'est pas a la meme position que le dernier
            posRand = rand() % (WIDTH-1); 

        switch (ennemiVoulu)
        {
        case BASIC:
            listEntites.emplace_back(make_unique<BasicEnnemi>(posRand, 0));    
            break;
        case RAPIDE:
            //listEntites.emplace_back(make_unique<RapideEnnemi>(posRand, 0));
            break;
        case TANK:
            //listEntites.emplace_back(make_unique<TankEnnemi>(posRand, 0));
            break;
        case ARTILLEUR:
            //listEntites.emplace_back(make_unique<ArtilleurEnnemi>(posRand, 0));
            break;
        case DIVEBOMBER:
            //listEntites.emplace_back(make_unique<DiveBomberEnnemi>(posRand, 0));
            break;
        }
    }*/
}


void Interface::progressionDifficulte()
{
    enemySpawnTimer++;

    if (score < 300)
    {

        if (enemySpawnTimer >= 70)          //on fait spawn une vague d'ennemis a toutes les 70 frames
        {
            enemySpawn(3, BASIC);   //on fait spawn 3 ennemis a chaque vague
            enemySpawnTimer = 0;        //on reset le timer pour pouvoir spanw la prochaine vague d'ennemis
        }
    }
	if (score >= 300 && score < 600)
	{
		if (enemySpawnTimer >= 60)          //on fait spawn une vague d'ennemis a toutes les 60 frames
		{
			enemySpawn(4, BASIC);   //on fait spawn 4 ennemis a chaque vague
			enemySpawnTimer = 0;        //on reset le timer pour pouvoir spanw la prochaine vague d'ennemis
		}
	}
	if (score >= 600)
	{
		if (enemySpawnTimer >= 40)          //on fait spawn une vague d'ennemis a toutes les 50 frames
		{
			enemySpawn(5, BASIC);   //on fait spawn 5 ennemis a chaque vague
			enemySpawnTimer = 0;        //on reset le timer pour pouvoir spanw la prochaine vague d'ennemis
		}
	}
}

//met a jour les entites a chaque frame
void Interface::updateEntites()
{
    vector<unique_ptr<Entite>> bufferBullets;  //on fait un buffer

	for (auto& e : listEntites)     //on parcourt la liste d'entites
    {
        if (e->enVie)
        {
			e->update();    //on update chaque entite a chaque frame

            // a toute les 10 frames les ennemis tirent
            if (e->type == ENNEMI && e->moveTimer % e->shootCooldown  == 0)    //on verifie si c'est un ennemi et si sont compteur pour tirer est a 0
				bufferBullets.emplace_back(make_unique<BasicBullet>(e->posX + e->largeur/2, e->posY + 1, false));           //on cree un bullet a la position de l'ennemi qu'on met un buffer temporaire pour 
		}                                                                                                                   // eviter de les ajouter a la liste d'entites pendant qu'on itere a travers d'elle
	}
	for (auto& bullet : bufferBullets) 
		listEntites.push_back(move(bullet));	//on ajoute les bullets du buffer a la liste d'entites
	
}


//gere les collisions entre les entites
void Interface::gererCollisions()
{
    for (auto& e : listEntites)
    {
        if (e->enVie)
        {
            //on verifie si un enemi ou un bullet est entre en collision avec le joueur et verifie que e n'est pas joueur
            if (joueur->enCollision(e->posX, e->posY) && e->symbole != '^')
            {
                if (e->type == ENNEMI && e->collisionJoueur == false)
                {
                    for (int i = 0; i < 2; i++)     //joueur perd 2 vies si il entre en collision avec un ennemi
                    {
                        if (joueur->nbVies > 0)
                            joueur->perdVie();

                        if (!joueur->enVie)
                            gameOver = true;
                    }
                    e->collisionJoueur = true;
                }
                else if (e->type == BULLET && e->collisionJoueur == false)     //si le joueur entre en collision avec une bullet ennemi il perd une vie
                {
                    if (joueur->nbVies > 0)
                        joueur->perdVie();

                    if (!joueur->enVie)
                        gameOver = true;

                    e->collisionJoueur = true;
                }

            }
            //on verifie si un bullet entre en collision avec un ennemi et si ell est un tir allie
            else if (e->symbole == '|' && e->allieOuEnnemi)
            {
                for (auto& e2 : listEntites)
                {
                    if (e2->enVie && e2->enCollision(e->posX, e->posY) && e2->symbole != '|')
                    {
                        e2->perdVie();
                        e->enVie = false;
                        if (!e2->enVie)
                            score += 10;

                    }
                }
            }
            else {
                e->collisionJoueur = false;
            }
        }
    }
}


//met a jour l'affichage de la console 
void Interface::updateAffichage()
{
	wchar_t buffer[HEIGHT + 3][WIDTH + 2];  // +3 pour les bordures et le score, +2 pour les bordures

    // Remplir le tampon avec des espaces (effacer l'écran)
    for (int y = 0; y < HEIGHT + 3; y++) {
        for (int x = 0; x < WIDTH + 2; x++) {
            buffer[y][x] = L' ';
        }
    }

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
            for (int y = 0; y < e->hauteur; y++)       
            {
                for (int x = 0; x < e->largeur; x++)
                {
                    buffer[e->posY + y][e->posX + x] = e->symbole;
                }
            }
        }
    }

	wstring scoreText;  //on cree un string pour le score

    if (gameOver)
        scoreText = L"Game Over!  Score: " + to_wstring(score);
    else
        scoreText = L"Score: " + to_wstring(score) + L"   Lives: " + to_wstring(joueur->nbVies);

	for (size_t i = 0; i < scoreText.size(); i++)   //on ajoute le score au buffer
        buffer[HEIGHT + 2][i] = scoreText[i];
    

	// on affiche chaque ligne du buffer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < HEIGHT + 3; y++) 
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
		progressionDifficulte();
        gererInput();
		updateEntites();
        gererCollisions();
        enleverEntites();
        updateAffichage();
        Sleep(25);
        //probablement autre chose
    }
	Sleep(2500);
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