#include "Interface.h"

Interface :: Interface()
{
    joueur = make_unique<Joueur>(WIDTH/2, HEIGHT-1);
    score = 0;
    gameOver = false;
    enemyDirection = 1;
    enemyMoveTimer = 0;

  
}

Interface :: ~Interface() {}

void Interface :: gererInput()
{
    if(_kbhit())
    {
        char input = _getch();

        if(input == 'a')
        {
            joueur->posX--;
        }
        else if(input == 'd')
        {
            joueur->posX++;
        }
        else if(input == ' ')
        {
           
        }
    }
}

void Interface :: updateAffichage()
{
    
}

void Interface :: updateEntites()
{
    
}

void Interface :: gererCollisions()
{
    
}

void Interface :: enleverEntites()
{
   
}

void Interface :: executionJeu()
{
    while(!gameOver)
    {
        gererInput();
        updateAffichage();
        updateEntites();
        gererCollisions();
        enleverEntites();
        //probablement autre chose
    }
}