#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

bool gameOver;
const int width = 50;
const int height = 20;
int x, y; // Player position
int score;
vector<int> enemyX;
vector<int> enemyY;
vector<bool> enemyActive;
vector<int> bulletX;
vector<int> bulletY;
vector<bool> bulletActive;
const char playerChar = '^';
const char enemyChar = 'O';
const char bulletChar = '|';
const char barrierChar = '#';
int barrierX[4] = {10, 20, 30, 40};
int barrierY = 15;
int barrierHealth[4] = {12, 5, 15, 50};

// Double-buffering
char buffer[height][width];

void Setup() {
    gameOver = false;
    x = width / 2;
    y = height - 1;
    score = 0;
    
    // Initialize enemies
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 3; j++) {
            enemyX.push_back(5 + i * 5);
            enemyY.push_back(2 + j * 2);
            enemyActive.push_back(true);
        }
    }
}

void ClearBuffer() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            buffer[i][j] = ' ';
        }
    }
}

void DrawToBuffer() {
    // Draw player
    buffer[y][x] = playerChar;

    // Draw enemies
    for(int i = 0; i < enemyActive.size(); i++) {
        if(enemyActive[i]) {
            buffer[enemyY[i]][enemyX[i]] = enemyChar;
        }
    }

    // Draw bullets
    for(int i = 0; i < bulletActive.size(); i++) {
        if(bulletActive[i]) {
            buffer[bulletY[i]][bulletX[i]] = bulletChar;
        }
    }

    // Draw barriers
    for(int b = 0; b < 4; b++) {
        if(barrierHealth[b] > 0) {
            for(int i = 0; i < 5; i++) {
                buffer[barrierY][barrierX[b] + i] = barrierChar;
            }
        }
    }
}

void Render() {
    // Move cursor to top-left corner
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

    // Draw top border
    for(int i = 0; i < width+2; i++)
        cout << "#";
    cout << endl;

    // Draw buffer
    for(int i = 0; i < height; i++) {
        cout << "#"; // Left border
        for(int j = 0; j < width; j++) {
            cout << buffer[i][j];
        }
        cout << "#" << endl; // Right border
    }

    // Draw bottom border
    for(int i = 0; i < width+2; i++)
        cout << "#";
    cout << endl;

    // Draw score
    cout << "Score: " << score << endl;
}

void Input() {
    if(_kbhit()) {
        char key = _getch();
        switch(key) {
            case 75: // Left arrow
                if(x > 0) x--;
                break;
            case 77: // Right arrow
                if(x < width-1) x++;
                break;
            case 32: // Spacebar
                // Fire bullet
                bulletX.push_back(x);
                bulletY.push_back(y-1);
                bulletActive.push_back(true);
                break;
            case 'q':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    static int enemyMoveTimer = 0;
    static int enemyDirection = 1;
    static int enemyShootTimer = 0;
    
    // Move bullets
    for(int i = 0; i < bulletActive.size(); i++) {
        if(bulletActive[i]) {
            bulletY[i]--;
            if(bulletY[i] < 0) {
                bulletActive[i] = false;
            }
        }
    }

    // Enemy movement
    if(enemyMoveTimer++ > 20) {
        bool edgeReached = false;
        for(int i = 0; i < enemyActive.size(); i++) {
            if(enemyActive[i]) {
                enemyX[i] += enemyDirection;
                if(enemyX[i] <= 0 || enemyX[i] >= width-1) {
                    edgeReached = true;
                }
            }
        }
        if(edgeReached) {
            enemyDirection *= -1;
            for(int i = 0; i < enemyActive.size(); i++) {
                if(enemyActive[i]) {
                    enemyY[i]++;
                }
            }
        }
        enemyMoveTimer = 0;
    }

    // Enemy shooting
    if(enemyShootTimer++ > 50) {
        vector<int> activeEnemies;
        for(int i = 0; i < enemyActive.size(); i++) {
            if(enemyActive[i]) activeEnemies.push_back(i);
        }
        if(!activeEnemies.empty()) {
            int shooter = activeEnemies[rand() % activeEnemies.size()];
            bulletX.push_back(enemyX[shooter]);
            bulletY.push_back(enemyY[shooter]+1);
            bulletActive.push_back(true);
        }
        enemyShootTimer = 0;
    }

    // Collision detection
    for(int i = 0; i < bulletActive.size(); i++) {
        if(bulletActive[i]) {
            // Player bullet hits enemy
            if(bulletY[i] < height/2) {
                for(int j = 0; j < enemyActive.size(); j++) {
                    if(enemyActive[j] && enemyX[j] == bulletX[i] && enemyY[j] == bulletY[i]) {
                        enemyActive[j] = false;
                        bulletActive[i] = false;
                        score += 100;
                    }
                }
            }
            
            // Bullet hits barrier
            for(int b = 0; b < 4; b++) {
                if(bulletY[i] == barrierY && bulletX[i] >= barrierX[b] && bulletX[i] < barrierX[b]+5) {
                    if(barrierHealth[b] > 0) {
                        barrierHealth[b]--;
                        bulletActive[i] = false;
                    }
                }
            }
        }
    }

    // Check if enemies reach bottom
    for(int i = 0; i < enemyActive.size(); i++) {
        if(enemyActive[i] && enemyY[i] >= barrierY-1) {
            gameOver = true;
            return;
        }
    }
}

int main() {
    srand(time(0));
    Setup();
    while(!gameOver) {
        ClearBuffer();
        DrawToBuffer();
        Render();
        Input();
        Logic();
        Sleep(1);
    }
    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}
