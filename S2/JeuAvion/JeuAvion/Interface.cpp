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

void Interface :: updateAffichage()
{
    // Efface l'écran (optionnel si on veut éviter un scintillement)
    //system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Afficher les bordures
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, pos);
    cout << '#' << string(WIDTH, '#') << "#\n";

    // Afficher les entités directement à leur position
    for (auto& e : listEntites) {
        if (e->enVie) {
            pos = { static_cast<SHORT>(e->posX + 1), static_cast<SHORT>(e->posY + 1) };
            SetConsoleCursorPosition(hConsole, pos);
            cout << e->symbole;
        }
    }

    // Afficher les bordures du bas et le score
    pos = {0, static_cast<SHORT>(HEIGHT + 1)};
    SetConsoleCursorPosition(hConsole, pos);
    cout << '#' << string(WIDTH, '#') << "#\n";

    pos = { 0, static_cast<SHORT>(HEIGHT + 2) };
    SetConsoleCursorPosition(hConsole, pos);
    cout << "Score: " << score << "   Lives: " << joueur->nbVies << "\n"; 
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
    hideCursor();
    while(!gameOver)
    {
        gererInput();
        updateAffichage();
        gererCollisions();
        enleverEntites();
        Sleep(50);
        //probablement autre chose
    }

    showCursor();
}