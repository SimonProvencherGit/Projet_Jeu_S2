#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 20;

class Entity {
public:
    int x, y;
    char symbol;
    bool active = true;

    Entity(int x, int y, char s) : x(x), y(y), symbol(s) {}
    virtual ~Entity() = default;
    virtual void update() {}
    virtual bool isColliding(int px, int py) const { return px == x && py == y; }
};

class Player : public Entity {
public:
    Player(int x, int y) : Entity(x, y, '^') {}
};

class Enemy : public Entity {
public:
    Enemy(int x, int y) : Entity(x, y, 'O') {}
};

class Bullet : public Entity {
public:
    bool isPlayerBullet;
    
    Bullet(int x, int y, bool isPlayer) 
        : Entity(x, y, '|'), isPlayerBullet(isPlayer) {}
    
    void update() override {
        if(isPlayerBullet) 
            y--;
        else 
            y++;
    }
};

class Barrier : public Entity {
public:
    int health = 5;
    
    Barrier(int x, int y) : Entity(x, y, '#') {}
    
    void update() override {
        if(health <= 0) active = false;
    }
};

class Game {
    unique_ptr<Player> player;
    vector<unique_ptr<Entity>> entities;
    vector<vector<Entity*>> grid;
    int score = 0;
    bool gameOver = false;
    int enemyDirection = 1;
    int enemyMoveTimer = 0;

    // Double-buffering
    char buffer[HEIGHT][WIDTH];

    public:
    Game() : grid(HEIGHT, vector<Entity*>(WIDTH, nullptr)) {
        setup();
    }

    void setup() {
        // Create player
        player = make_unique<Player>(WIDTH/2, HEIGHT-1);

        // Create enemies
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 3; j++) {
                entities.emplace_back(
                    make_unique<Enemy>(5 + i * 5, 2 + j * 2)
                );
            }
        }

        // Create barriers
        int barrierX[4] = {10, 20, 30, 40};
        for(int x : barrierX) {
            entities.emplace_back(make_unique<Barrier>(x, 15));
        }
    }

    void updateGrid() {
        // Clear grid
        for(auto& row : grid)
            fill(row.begin(), row.end(), nullptr);

        // Add player
        grid[player->y][player->x] = player.get();

        // Add entities
        for(auto& e : entities) {
            if(e->active) {
                if(e->y >= 0 && e->y < HEIGHT && e->x >= 0 && e->x < WIDTH) {
                    grid[e->y][e->x] = e.get();
                }
            }
        }
    }

    void handleInput() {
        if(_kbhit()) {
            int key = _getch();
            switch(key) {
                case 75: if(player->x > 0) player->x--; break;
                case 77: if(player->x < WIDTH-1) player->x++; break;
                case 32: // Spacebar
                    entities.emplace_back(
                        make_unique<Bullet>(player->x, player->y-1, true)
                    );
                    break;
                case 'q': gameOver = true; break;
            }
        }
    }

    void updateEntities() {
        // Update all entities
        for(auto& e : entities) {
            e->update();
            if(auto bullet = dynamic_cast<Bullet*>(e.get())) {
                if(bullet->y < 0 || bullet->y >= HEIGHT) {
                    bullet->active = false;
                }
            }
        }

        // Enemy movement
        if(enemyMoveTimer++ > 20) {
            bool edgeReached = false;
            for(auto& e : entities) {
                if(auto enemy = dynamic_cast<Enemy*>(e.get())) {
                    enemy->x += enemyDirection;
                    if(enemy->x <= 0 || enemy->x >= WIDTH-1) {
                        edgeReached = true;
                    }
                }
            }
            
            if(edgeReached) {
                enemyDirection *= -1;
                for(auto& e : entities) {
                    if(auto enemy = dynamic_cast<Enemy*>(e.get())) {
                        enemy->y++;
                    }
                }
            }
            enemyMoveTimer = 0;
        }

        // Enemy shooting
        static int enemyShootTimer = 0;
        if(enemyShootTimer++ > 50) {
            vector<Enemy*> activeEnemies;
            for(auto& e : entities) {
                if(dynamic_cast<Enemy*>(e.get()) && e->active) {
                    activeEnemies.push_back(static_cast<Enemy*>(e.get()));
                }
            }
            if(!activeEnemies.empty()) {
                Enemy* shooter = activeEnemies[rand() % activeEnemies.size()];
                entities.emplace_back(
                    make_unique<Bullet>(shooter->x, shooter->y+1, false)
                );
            }
            enemyShootTimer = 0;
        }

        // Collision detection
        checkCollisions();
        cleanupEntities();
    }

    void checkCollisions() {
        // Check player bullets hitting enemies
        for(auto& bullet : entities) {
            if(auto b = dynamic_cast<Bullet*>(bullet.get())) {
                if(b->isPlayerBullet) {
                    for(auto& e : entities) {
                        if(auto enemy = dynamic_cast<Enemy*>(e.get())) {
                            if(enemy->active && enemy->isColliding(b->x, b->y)) {
                                enemy->active = false;
                                bullet->active = false;
                                score += 100;
                            }
                        }
                    }
                }
            }
        }
    }

    void cleanupEntities() {
        // Remove inactive entities
        entities.erase(
            remove_if(entities.begin(), entities.end(),
                [](const unique_ptr<Entity>& e) { return !e->active; }),
            entities.end()
        );
    }

    void clearBuffer() {
        for(int y = 0; y < HEIGHT; y++) {
            for(int x = 0; x < WIDTH; x++) {
                buffer[y][x] = ' ';
            }
        }
    }

    void drawToBuffer() {
        // Draw player
        buffer[player->y][player->x] = player->symbol;

        // Draw entities
        for(auto& e : entities) {
            if(e->active && e->y >= 0 && e->y < HEIGHT && e->x >= 0 && e->x < WIDTH) {
                buffer[e->y][e->x] = e->symbol;
            }
        }
    }

    void render() {
        // Move cursor to top-left corner
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

        // Draw top border
        for(int i = 0; i < WIDTH+2; i++) cout << '#';
        cout << endl;

        // Draw buffer
        for(int y = 0; y < HEIGHT; y++) {
            cout << '#';
            for(int x = 0; x < WIDTH; x++) {
                cout << buffer[y][x];
            }
            cout << "#\n";
        }

        // Draw bottom border and score
        for(int i = 0; i < WIDTH+2; i++) cout << '#';
        cout << endl;
        cout << "Score: " << score << endl;
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    }

    void showCursor() {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    }

    void run() {
        hideCursor();  // Hide the cursor at the start of the game

        while(!gameOver) {
            clearBuffer();
            updateGrid();
            drawToBuffer();
            render();
            handleInput();
            updateEntities();
            Sleep(5);

            // Check if enemies reached bottom
            for(auto& e : entities) {
                if(auto enemy = dynamic_cast<Enemy*>(e.get())) {
                    if(enemy->y >= HEIGHT-2) {
                        gameOver = true;
                        break;
                    }
                }
            }
        }

        showCursor();  // Restore the cursor when the game ends
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run();
    return 0;
}