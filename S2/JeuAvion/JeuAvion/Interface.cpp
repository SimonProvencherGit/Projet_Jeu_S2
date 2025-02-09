#include "Interface.h"

Interface :: Interface()
{
    score = 0;
    gameOver = false;
    //enemyDirection = 1;
    enemySpawnTimer = 0;

    listEntites.emplace_back(make_unique<Joueur>(WIDTH / 2, HEIGHT - 1));
    joueur = static_cast<Joueur*>(listEntites.back().get());

}

Interface :: ~Interface() {}

void Interface :: gererInput()
{
   // joueur->ancienX = joueur->posX;
   // joueur->ancienY = joueur->posY;

    if (GetAsyncKeyState(VK_LEFT) < 0) 
        if (joueur->posX > 0) 
            joueur->posX--;
    
    if (GetAsyncKeyState(VK_RIGHT) < 0) 
        if (joueur->posX < WIDTH - 1) 
            joueur->posX++;
    
    if (GetAsyncKeyState(VK_UP) < 0) 
        if (joueur->posY > HEIGHT / 2) 
            joueur->posY--;
   
    if (GetAsyncKeyState(VK_DOWN) < 0) 
        if (joueur->posY < HEIGHT - 1) 
            joueur->posY++;
    if (GetAsyncKeyState(VK_SPACE) < 0) {
        //if (shootCooldown <= 0) {
            listEntites.emplace_back(make_unique<BasicBullet>(joueur->posX, joueur->posY - 1, true));
            //shootCooldown = SHOOT_COOLDOWN;
       //}
    }
    if (GetAsyncKeyState('Q') < 0) {
        gameOver = true;
    }
}

//determine quand le enenmi spawns a des positions random en haut du tableau
void Interface::enemySpawn(int nbEnnemi)
{
	int posRand = rand() % WIDTH;
    int anciennePos;
	enemySpawnTimer++;
	if (enemySpawnTimer >= 50) {
		
        for (int i = 0; i < nbEnnemi; i++)
        {
            anciennePos = posRand;
            posRand = rand() % WIDTH;
            while (posRand == anciennePos)
                posRand = rand() % WIDTH;

            listEntites.emplace_back(make_unique<BasicEnnemi>(posRand, 0));
        }

		//spawn un enemi a une differente place que le dernier 

		enemySpawnTimer = 0;
	}
}

void Interface::updatePosEntites()
{
	for (auto& e : listEntites) {
		e->update();
	}
}

void Interface::updateAffichage()
{
    wchar_t buffer[HEIGHT + 3][WIDTH + 2];

    // Remplir le tampon avec des espaces (effacer l'écran)
    for (int y = 0; y < HEIGHT + 3; y++) {
        for (int x = 0; x < WIDTH + 2; x++) {
            buffer[y][x] = L' ';
        }
    }

    // Mettre à jour les bordures
    for (int x = 0; x < WIDTH + 2; x++) {
        buffer[0][x] = L'#';
        buffer[HEIGHT + 1][x] = L'#';
    }
    for (int y = 1; y <= HEIGHT; y++) {
        buffer[y][0] = L'#';
        buffer[y][WIDTH + 1] = L'#';
    }

    // Mettre à jour les entités vivantes
    for (auto& e : listEntites) 
    {
        if (e->enVie) 
            buffer[e->posY + 1][e->posX + 1] = e->symbole;  // +1 pour les bordures
    }

    // Afficher les bordures du bas et le score
    wstring scoreText = L"Score: " + to_wstring(score) + L"   Lives: " + to_wstring(joueur->nbVies);
    for (size_t i = 0; i < scoreText.size(); i++) {
        buffer[HEIGHT + 2][i] = scoreText[i];
    }

	// on affiche chaque ligne du buffer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < HEIGHT + 3; y++) 
    {
        COORD bufferCoord = { 0, static_cast<SHORT>(y) };
        DWORD charsWritten;
        // Write the entire row, including borders and trailing spaces
        WriteConsoleOutputCharacterW(hConsole, buffer[y], WIDTH + 2, bufferCoord, &charsWritten);
    }
}

void Interface :: gererCollisions()
{
	for (auto& e : listEntites)
	{
		if (e->enVie)
		{
		    //on verifie si un enemi ou un bullet est entre en collision avec le joueur et verifie que e n'est pas joueur
            if (e->enCollision(joueur->posX, joueur->posY) && e->symbole != '^')
            {
                joueur->perdVie();
                if (!joueur->enVie)
                    gameOver = true;
            }

		}
	}
}

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

void Interface :: hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Interface:: showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Interface :: executionJeu()
{
    hideCursor();
    while(!gameOver)
    {
		enemySpawn(3);
        gererInput();
		updatePosEntites();
        gererCollisions();
        enleverEntites();
        updateAffichage();
        Sleep(25);
        //probablement autre chose
    }

    showCursor();
}


void setConsoleSize() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting console handle" << std::endl;
        return;
    }

    // Set the buffer size first
    COORD bufferSize = { WIDTH, HEIGHT };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Then set the window size
    SMALL_RECT windowSize = { 0, 0, WIDTH+3, HEIGHT+3};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

