// v7 - Collision Detection
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Player;
class Bullet;
class Enemy;
void resetKeyClicks();

// GLOBALS - START
int RES_X = 800;
int RES_Y = 600;
sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "This is not a game. This is serious business.");
int frame = 0;

bool key_left = false;
bool key_right = false;
bool key_space_click = false;
bool key_z_click = false;

Player* player;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
// GLOBALS - END

class Bullet {
public:
    float x, y, vx, vy, ax, ay;
    bool isActive;
    float radius;

    Bullet(float x, float y, float vx, float vy, float ax, float ay, float radius)
    : x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay), radius(radius) {
        isActive = true;
    }

    void update() {
        vx += ax;
        vy += ay;
        x += vx;
        y += vy;

        if (isOffScreen()) {
            remove();
            return;
        }
    }

    void remove() {
        isActive = false;
    }

    bool isOffScreen() {
        float tolerance = 100.f;
        return x < -radius - tolerance ||
                y < -radius - tolerance ||
                x > RES_X + radius + tolerance || 
                y > RES_Y + radius + tolerance;
    }

    void draw() {
        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Yellow);
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
        window.draw(shape);
    }
};

class Player {
public:
    float x, y;
    float vx, vy;
    bool isOnGround;
    bool isFacingRight;

    int width = 25;
    int height = 40;
    int speed = 3;
    float gravity = 0.4f;
    float jumpSpeed = 10;

    Player(float x, float y): x(x), y(y) {
        vx = 0;
        vy = 0;
        isOnGround = false;
        isFacingRight = true;
    }

    void update() {
        vx = 0;
        vy += gravity;
        if (key_left) vx -= speed;
        if (key_right) vx += speed;
        if (key_space_click) {
            if (isOnGround) vy = -jumpSpeed;
        }
        
        if (key_left && !key_right) isFacingRight = false;
        if (key_right && !key_left) isFacingRight = true;
        if (key_z_click) shootBullet();

        isOnGround = false;

        x += vx;
        if (y + vy >= RES_Y - height/2) {
            y = RES_Y - height/2;
            vy = 0;
            isOnGround = true;
        } else {
            y += vy;
        }

    }

    void shootBullet() {
        float bulletVx = 10;
        if (!isFacingRight) bulletVx *= -1;

        bullets.push_back(Bullet(x, y, bulletVx, 0, 0, 0, 5));
    }

    void draw() {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Blue);
        shape.setSize(sf::Vector2f(width, height));
        shape.setOrigin(width/2, height/2);
        shape.setPosition(x, y);
        window.draw(shape);
    }
};


class Enemy {
public:
    float x, y, vx, vy;
    bool isActive;
    int nextMovementCountdown;
    int hp;

    int width = 40;
    int height = 40;
    float speed = 2;

    Enemy(float x, float y): x(x), y(y) {
        isActive = true;
        nextMovementCountdown = 0;
        hp = 100;
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) remove();
    }

    void remove() {
        isActive = false;
    }

    void checkCollision() {
        for (auto& bullet : bullets) {
            if (!bullet.isActive) continue;

            if (x - width/2 <= bullet.x + bullet.radius &&
                bullet.x - bullet.radius <= x + width/2 &&
                y - height/2 <= bullet.y + bullet.radius &&
                bullet.y - bullet.radius <= y + height/2) {

                bullet.remove();
                takeDamage(20);
            }
        }
    }

    void updateVelocity() {
        if (nextMovementCountdown > 0) {
            --nextMovementCountdown;
            return;
        } else {
            nextMovementCountdown = rand()%120 + 60;
        }

        double angle = rand()%1000 * (2*3.1415926535/1000);
        vx = speed*cos(angle);
        vy = speed*sin(angle);

        if (x > RES_X && vx > 0) vx *= -1;
        if (x < 0 && vx < 0) vx *= -1;
        if (y > RES_Y && vy > 0) vy *= -1;
        if (y < 2*RES_Y/3 && vy < 0) vy *= -1;
    }

    void update() {
        updateVelocity();
        x += vx;
        y += vy;
        checkCollision();
    }

    void draw() {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Red);
        shape.setSize(sf::Vector2f(width, height));
        shape.setOrigin(width/2, height/2);
        shape.setPosition(x, y);
        window.draw(shape);
    }
};



void initialiseGame() {
    if (player != nullptr) delete player;
    bullets.clear();
    enemies.clear();

    player = new Player(RES_X/2, RES_Y/4);
    frame = 0;
}

void updateGame() {
    player->update();

    for (auto& bullet : bullets) {
        if (bullet.isActive) bullet.update();
    }

    for (auto& enemy : enemies) {
        if (enemy.isActive) enemy.update();
    }

    if (frame%300 == 0) {
        enemies.push_back(Enemy(rand()%RES_X, rand()%RES_Y/3));
    }

    resetKeyClicks();
    ++frame;
}

void drawGameFrame() {
    player->draw();

    for (auto& bullet : bullets) {
        if (bullet.isActive) bullet.draw();
    }

    for (auto& enemy : enemies) {
        if (enemy.isActive) enemy.draw();
    }
}

void keyDown(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::Left: key_left = true; break;
        case sf::Keyboard::Right: key_right = true; break;
        case sf::Keyboard::Space: key_space_click = true; break;
        case sf::Keyboard::Z: key_z_click = true; break;
    }
}

void keyUp(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::Left: key_left = false; break;
        case sf::Keyboard::Right: key_right = false; break;
    }
}

void resetKeyClicks() {
    key_space_click = false;
    key_z_click = false;
}

void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window.close();
            break;
        }
        case sf::Event::KeyPressed: {
            keyDown(event.key.code);
            break;
        }
        case sf::Event::KeyReleased: {
            keyUp(event.key.code);
            break;
        }
    }
}

int main() {
    srand(time(0));

    sf::Clock clock;
    float frameTime = 1/60.0f;
    float dTime = 0;

    initialiseGame();

    while(window.isOpen()) {
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        // Event handling
        sf::Event event;
        while(window.pollEvent(event)) {
            processEvent(event);
        }

        // Safeguard (slowdown) to prevent game from lagging to death
        if (dTime > 5*frameTime) dTime = 5*frameTime;

        // Update game
        while (dTime > frameTime) {
            dTime -= frameTime;
            updateGame();
        }

        // Draw frame
        window.clear();
        drawGameFrame();
        window.display();
    }

    return 0;
}