#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>
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
    int lives;

    Player(int x, int y) : Entity(x, y, '^'), lives(3) {}

    void loseLife() {
        lives--;
        if (lives <= 0) {
            active = false;
        }
    }
};

class Enemy : public Entity {
public:
    Enemy(int x, int y) : Entity(x, y, 'O') {}
};

class Bullet : public Entity {
public:
    bool isPlayerBullet;

    Bullet(int x, int y, bool isPlayer) : Entity(x, y, '|'), isPlayerBullet(isPlayer) {}

    void update() override {
        if (isPlayerBullet)
            y--;
        else
            y++;
    }
};

class Barrier : public Entity {
public:
    int health;

    Barrier(int x, int y) : Entity(x, y, '#'), health(5) {}

    void update() override {
        if (health <= 0) active = false;
    }
};

class Game {
    vector<unique_ptr<Entity>> entities;
    Player* playerPtr;
    int score = 0;
    bool gameOver = false;
    int enemyDirection = 1;
    int enemyMoveTimer = 0;

public:
    Game() {
        setup();
    }

    void setup() {
        // Create enemies
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                entities.emplace_back(make_unique<Enemy>(5 + i * 5, 2 + j * 2));
            }
        }

        // Create barriers
        int barrierX[4] = { 10, 20, 30, 40 };
        for (int x : barrierX) {
            entities.emplace_back(make_unique<Barrier>(x, 15));
        }

        // Create player and add to entities
        entities.emplace_back(make_unique<Player>(WIDTH / 2, HEIGHT - 1));
        playerPtr = static_cast<Player*>(entities.back().get());
    }

    void handleInput() {
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
            case 75: // Left arrow
                if (playerPtr->x > 0) playerPtr->x--;
                break;
            case 77: // Right arrow
                if (playerPtr->x < WIDTH - 1) playerPtr->x++;
                break;
            case 32: // Space to shoot
                entities.emplace_back(make_unique<Bullet>(playerPtr->x, playerPtr->y - 1, true));
                break;
            case 'q': // Quit
                gameOver = true;
                break;
            }
        }
    }

    void updateEntities() {
        for (auto& e : entities) {
            e->update();
            if (auto bullet = dynamic_cast<Bullet*>(e.get())) {
                if (bullet->y < 0 || bullet->y >= HEIGHT)
                    bullet->active = false;
            }
        }

        // Enemy movement
        if (enemyMoveTimer++ > 1) {
            bool edgeReached = false;
            for (auto& e : entities) {
                if (auto enemy = dynamic_cast<Enemy*>(e.get())) {
                    enemy->x += enemyDirection;
                    if (enemy->x <= 0 || enemy->x >= WIDTH - 1)
                        edgeReached = true;
                }
            }
            if (edgeReached) {
                enemyDirection *= -1;
                for (auto& e : entities) {
                    if (auto enemy = dynamic_cast<Enemy*>(e.get()))
                        enemy->y++;
                }
            }
            enemyMoveTimer = 0;
        }

        // Enemy shooting
        static int enemyShootTimer = 0;
        if (enemyShootTimer++ > 10) {
            vector<Enemy*> activeEnemies;
            for (auto& e : entities) {
                if (auto enemy = dynamic_cast<Enemy*>(e.get()))
                    if (enemy->active)
                        activeEnemies.push_back(enemy);
            }
            if (!activeEnemies.empty()) {
                Enemy* shooter = activeEnemies[rand() % activeEnemies.size()];
                entities.emplace_back(make_unique<Bullet>(shooter->x, shooter->y + 1, false));
            }
            enemyShootTimer = 0;
        }

        checkCollisions();
        cleanupEntities();
    }

    void checkCollisions() {
        for (auto& bulletEntity : entities) {
            if (auto b = dynamic_cast<Bullet*>(bulletEntity.get())) {
                if (b->isPlayerBullet) {
                    for (auto& e : entities) {
                        if (auto enemy = dynamic_cast<Enemy*>(e.get())) {
                            if (enemy->active && enemy->isColliding(b->x, b->y)) {
                                enemy->active = false;
                                b->active = false;
                                score += 100;
                            }
                        }
                        if (auto barrier = dynamic_cast<Barrier*>(e.get())) {
                            if (barrier->active && barrier->isColliding(b->x, b->y)) {
                                b->active = false;
                                barrier->health--;
                                if (barrier->health <= 0)
                                    barrier->active = false;
                            }
                        }
                    }
                }
                else {
                    if (playerPtr->active && playerPtr->isColliding(b->x, b->y)) {
                        b->active = false;
                        playerPtr->loseLife();
                    }
                    for (auto& e : entities) {
                        if (auto barrier = dynamic_cast<Barrier*>(e.get())) {
                            if (barrier->active && barrier->isColliding(b->x, b->y)) {
                                b->active = false;
                                barrier->health--;
                                if (barrier->health <= 0)
                                    barrier->active = false;
                            }
                        }
                    }
                }
            }
        }
    }

    void cleanupEntities() {
        entities.erase(
            remove_if(entities.begin(), entities.end(),
                [this](const unique_ptr<Entity>& e) {
                    return !e->active && e.get() != playerPtr;
                }
            ),
            entities.end()
        );
    }

    void render() {
        // Move cursor to top-left
        COORD cursorPosition = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

        // Draw top border
        cout << '#' << string(WIDTH, '#') << "#\n";

        // Draw each row
        for (int y = 0; y < HEIGHT; y++) {
            cout << '#';
            for (int x = 0; x < WIDTH; x++) {
                char cell = ' ';
                for (auto& e : entities) {
                    if (e->active && e->x == x && e->y == y) {
                        cell = e->symbol;
                    }
                }
                cout << cell;
            }
            cout << "#\n";
        }

        // Draw bottom border and score
        cout << '#' << string(WIDTH, '#') << "#\n";
        cout << "Score: " << score << "   Lives: " << playerPtr->lives << "\n";
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
        hideCursor();
        while (!gameOver) {
            render();
            handleInput();
            updateEntities();
            Sleep(50);

            // Check if enemies reached the bottom
            for (auto& e : entities) {
                if (auto enemy = dynamic_cast<Enemy*>(e.get())) {
                    if (enemy->y >= HEIGHT - 2) {
                        gameOver = true;
                        break;
                    }
                }
            }

            if (!playerPtr->active)
                gameOver = true;
        }
        showCursor();
        cout << "Game Over! Final Score: " << score << "\n";
        Sleep(3000);
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run();
    return 0;
}