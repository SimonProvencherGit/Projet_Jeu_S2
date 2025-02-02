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

    // Le joueur perd une vie lorsqu'il est touché
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
    
    Bullet(int x, int y, bool isPlayer) 
        : Entity(x, y, '|'), isPlayerBullet(isPlayer) {}
    
    void update() override {
        // Si c'est une balle du joueur, elle se déplace vers le haut ; sinon vers le bas.
        if(isPlayerBullet) 
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
        // Si la barrière n'a plus de santé, on la désactive.
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

    // Double-buffer pour le rendu
    char buffer[HEIGHT][WIDTH];

public:
    Game() : grid(HEIGHT, vector<Entity*>(WIDTH, nullptr)) {
        setup();
    }

    void setup() {
        // Création du joueur au centre en bas de l'écran
        player = make_unique<Player>(WIDTH/2, HEIGHT-1);

        // Création d'une formation d'ennemis (5 colonnes x 3 lignes)
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                entities.emplace_back(
                    make_unique<Enemy>(5 + i * 5, 2 + j * 2)
                );
            }
        }

        // Création de barrières à des positions fixes
        int barrierX[4] = {10, 20, 30, 40};
        for (int x : barrierX) {
            entities.emplace_back(make_unique<Barrier>(x, 15));
        }
    }

    void updateGrid() {
        // Réinitialise la grille
        for (auto& row : grid)
            fill(row.begin(), row.end(), nullptr);

        // Place le joueur (s'il est actif)
        if (player->active)
            grid[player->y][player->x] = player.get();

        // Place les autres entités actives dans la grille
        for (auto& e : entities) {
            if (e->active && e->y >= 0 && e->y < HEIGHT && e->x >= 0 && e->x < WIDTH) {
                grid[e->y][e->x] = e.get();
            }
        }
    }

    void handleInput() {
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
                case 75: // Flèche gauche
                    if (player->x > 0) player->x--;
                    break;
                case 77: // Flèche droite
                    if (player->x < WIDTH - 1) player->x++;
                    break;
                case 32: // Espace pour tirer
                    entities.emplace_back(
                        make_unique<Bullet>(player->x, player->y - 1, true)
                    );
                    break;
                case 'q': // Quitter le jeu
                    gameOver = true;
                    break;
            }
        }
    }

    void updateEntities() {
        // Mise à jour de chaque entité
        for (auto& e : entities) {
            e->update();
            // Pour les balles, si elles sortent de l'écran, on les désactive
            if (auto bullet = dynamic_cast<Bullet*>(e.get())) {
                if (bullet->y < 0 || bullet->y >= HEIGHT)
                    bullet->active = false;
            }
        }

        // Mouvement collectif des ennemis
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

        // Tir aléatoire des ennemis
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
                entities.emplace_back(
                    make_unique<Bullet>(shooter->x, shooter->y + 1, false)
                );
            }
            enemyShootTimer = 0;
        }

        // Gestion des collisions
        checkCollisions();
        cleanupEntities();
    }

    void checkCollisions() {
        // Parcours de toutes les entités pour gérer les collisions
        for (auto& bulletEntity : entities) {
            if (auto b = dynamic_cast<Bullet*>(bulletEntity.get())) {
                if (b->isPlayerBullet) {
                    // Collision : balle du joueur touche un ennemi
                    for (auto& e : entities) {
                        if (auto enemy = dynamic_cast<Enemy*>(e.get())) {
                            if (enemy->active && enemy->isColliding(b->x, b->y)) {
                                enemy->active = false;
                                b->active = false;
                                score += 100;
                            }
                        }
                    }
                    // Collision : balle du joueur touche une barrière
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
                } else {
                    // Collision : balle ennemie touche le joueur
                    if (player->active && player->isColliding(b->x, b->y)) {
                        b->active = false;
                        player->loseLife();
                    }
                    // Collision : balle ennemie touche une barrière
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
        // Supprime les entités désactivées
        entities.erase(
            remove_if(entities.begin(), entities.end(),
                [](const unique_ptr<Entity>& e) { return !e->active; }
            ),
            entities.end()
        );
    }

    void clearBuffer() {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                buffer[y][x] = ' ';
            }
        }
    }

    void drawToBuffer() {
        // Dessine le joueur dans le buffer
        if (player->active && player->y >= 0 && player->y < HEIGHT &&
            player->x >= 0 && player->x < WIDTH)
            buffer[player->y][player->x] = player->symbol;

        // Dessine les autres entités
        for (auto& e : entities) {
            if (e->active && e->y >= 0 && e->y < HEIGHT &&
                e->x >= 0 && e->x < WIDTH)
                buffer[e->y][e->x] = e->symbol;
        }
    }

    void render() {
        // Repositionne le curseur en haut à gauche pour le double-buffering
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

        // Dessine la bordure supérieure
        for (int i = 0; i < WIDTH + 2; i++) cout << '#';
        cout << "\n";

        // Dessine le contenu du buffer avec des bordures latérales
        for (int y = 0; y < HEIGHT; y++) {
            cout << '#';
            for (int x = 0; x < WIDTH; x++) {
                cout << buffer[y][x];
            }
            cout << "#\n";
        }

        // Dessine la bordure inférieure, le score et le nombre de vies restantes
        for (int i = 0; i < WIDTH + 2; i++) cout << '#';
        cout << "\n";
        cout << "Score: " << score << "   Lives: " << player->lives << "\n";
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
            clearBuffer();
            updateGrid();
            drawToBuffer();
            render();
            handleInput();
            updateEntities();
            Sleep(1);

            // Si un ennemi atteint presque le bas de l'écran, on termine le jeu.
            for (auto& e : entities) {
                if (auto enemy = dynamic_cast<Enemy*>(e.get())) {
                    if (enemy->y >= HEIGHT - 2) {
                        gameOver = true;
                        break;
                    }
                }
            }
            // Si le joueur a perdu toutes ses vies, on termine le jeu.
            if (!player->active)
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
