#include "Interface.h"

Interface :: Interface()
{
    score = 0;
    gameOver = false;
    enemyDirection = 1;
    enemyMoveTimer = 0;

    listEntites.emplace_back(make_unique<Joueur>(WIDTH / 2, HEIGHT - 1));
    joueur = static_cast<Joueur*>(listEntites.back().get());

}

Interface :: ~Interface() {}

void Interface :: gererInput()
{
    if (_kbhit()) {
        int key = _getch();
		//cout << key << endl;
		joueur->ancienX = joueur->posX;
		joueur->ancienY = joueur->posY;

        switch (key) {
        case 75:    //gauche
            if (joueur->posX > 0) 
                joueur->posX--;
            break;
        case 77:    //droite
            if (joueur->posX < WIDTH - 1) 
                joueur->posX++;
            break;
        case 72:   //haut
            if (joueur->posY > HEIGHT/2) 
                joueur->posY--;        //pos y -- pour decendre car le tableau commence du haut vers le bas
            break;
        case 80:    //bas
            if (joueur->posY < HEIGHT -1)
                joueur->posY++;        
            break;
        case 32:    //espace
			listEntites.emplace_back(make_unique<Bullet>(joueur->posX, joueur->posY - 1, true));        //ajoute une balle tiree par le joueur dans le tableau d'entites 
            break;
        case 'q':
            gameOver = true;
            break;
        }
        //cout << "posX: " << joueur->posX << " posY: " << joueur->posY << endl;
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
    for (auto& e : listEntites) {
        if (e->enVie) {
            buffer[e->posY + 1][e->posX + 1] = e->symbole;  // +1 pour les bordures
        }
    }

    // Afficher les bordures du bas et le score
    wstring scoreText = L"Score: " + to_wstring(score) + L"   Lives: " + to_wstring(joueur->nbVies);
    for (size_t i = 0; i < scoreText.size(); i++) {
        buffer[HEIGHT + 2][i] = scoreText[i];
    }

    // Afficher le tampon à la console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { WIDTH + 2, HEIGHT + 3 };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, WIDTH + 1, HEIGHT + 2 };
    DWORD charsWritten;
    WriteConsoleOutputCharacterW(hConsole, &buffer[0][0], (WIDTH + 2) * (HEIGHT + 3), bufferCoord, &charsWritten);
}

void Interface :: gererCollisions()
{
    
}

void Interface :: enleverEntites()
{
   
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
    
    while(!gameOver)
    {
		hideCursor();
        gererInput();
		updatePosEntites();
        updateAffichage();
        gererCollisions();
        enleverEntites();
        Sleep(30);
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
    SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

