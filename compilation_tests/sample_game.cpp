#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

// Globals
sf::RenderWindow* window;
int RES_X = 800;
int RES_Y = 600;

bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;
bool key_z = false;

class Bullet;
std::vector<Bullet> bullets;

// Game Logic
class Player;

class Bullet {
private:
    int x, y, vx, vy;

public:
    static const int width = 70, height = 3;
    bool isActive;

    Bullet(): isActive(false) {}
    Bullet(int x, int y, int vx, int vy): x(x), y(y), vx(vx), vy(vy), isActive(true) {}
    
    void update() {
        if (!isActive) return;
        x += vx;
        y += vy;
        if (x < -width || x > RES_X + width ||
            y < -height || y > RES_Y + height) isActive = false;
    }

    void draw() {
        if (!isActive) return;

        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Cyan);
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x-width/2, y-height/2);
        window->draw(shape);
    }
};


class Cannon {
private:
    const int radius = 5;
    int x, y;
    bool isBehind;
    float angleDiff;

public:
    Cannon(float angleDiff): x(0), y(0), isBehind(false), angleDiff(angleDiff) {}

    void updatePosition(int p_x, int p_y, float p_angle) {
        const float xRadius = 18, yRadius = 30;
        float angle = p_angle + angleDiff;
        x = p_x + xRadius*cos(angle);
        y = p_y + yRadius*sin(angle);
        isBehind = (x > p_x);
    }

    void draw(bool _behind) {
        if (_behind != isBehind) return;
        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Yellow);
        shape.setRadius(radius);
        shape.setPosition(x-radius, y-radius);
        window->draw(shape);
    }

    void fire(int py) {
        int vy = (y - py)/17;
        bullets.push_back(Bullet(x,y,45,vy));
    }
};

class Player {
private:
    const int width = 40, height = 25, speed = 5;
    const float minAngleVel = 0.02f, maxAngleVel = 1.f, minFiringAngleVel = 0.2f;
    const float angleAcc = 0.003f, angleDec = 0.006f;
    const size_t nCannons = 5;
    const float TWO_PI = 2*3.1415926535f;
    int x, y;
    float angle, angleVel;
    size_t nextCannon;
    std::vector<Cannon> cannons;

public:
    Player(int x, int y): x(x),y(y), angle(0.3f), angleVel(minAngleVel), nextCannon(0) {
        for (size_t i=0;i<nCannons;++i) {
            cannons.push_back(Cannon(TWO_PI*i/nCannons));
        }
    }

    void update() {
        int vx = 0, vy = 0;
        if (key_up) vy -= speed;
        if (key_down) vy += speed;
        if (key_right) vx += speed;
        if (key_left) vx -= speed;
        x += vx;
        y += vy;
        if (key_z) {
            angleVel += angleAcc;
            if (angleVel > maxAngleVel) angleVel = maxAngleVel;
        } else {
            angleVel -= angleDec;
            if (angleVel < minAngleVel) angleVel = minAngleVel;
        }

        angle += angleVel;

        for (size_t i=0;i<nCannons;++i) {
            cannons[i].updatePosition(x,y,angle);
        }
        if (angleVel >= minFiringAngleVel && key_z) {
            cannons[nextCannon].fire(y);
            cannons[(nextCannon+3)%nCannons].fire(y);
            nextCannon = (nextCannon+1)%nCannons;
        }
    }

    void draw() {
        for (size_t i=0;i<nCannons;++i) {
            cannons[i].draw(true);
        }

        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Blue);
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x-width/2, y-height/2);
        window->draw(shape);

        for (size_t i=0;i<nCannons;++i) {
            cannons[i].draw(false);
        }
    }
};
Player* player;

void initialiseGame() {
    player = new Player(200,RES_Y/2);
}

void update() {
    player->update();

    size_t nActive = 0;
    for (size_t i=0,n=bullets.size();i<n;++i) {
        bullets[i].update();
        if (bullets[i].isActive) ++nActive;
    }

    // If too many inactive bullets, clean up the inactive bullets.
    if (nActive < bullets.size()/2) {
        size_t curr = 0;
        for (size_t i=0,n=bullets.size();i<n;++i) {
            if (bullets[i].isActive) {
                bullets[curr] = bullets[i];
                ++curr;
            }
        }
        bullets.resize(curr);
    }
}

void drawGameFrame() {
    player->draw();
    for (size_t i=0,n=bullets.size();i<n;++i) {
        bullets[i].draw();
    }
}



// Base Code
void keyDown(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::Up: key_up = true; break;
        case sf::Keyboard::Down: key_down = true; break;
        case sf::Keyboard::Left: key_left = true; break;
        case sf::Keyboard::Right: key_right = true; break;
        case sf::Keyboard::Z: key_z = true; break;
        default: break;
    }
}

void keyUp(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::Up: key_up = false; break;
        case sf::Keyboard::Down: key_down = false; break;
        case sf::Keyboard::Left: key_left = false; break;
        case sf::Keyboard::Right: key_right = false; break;
        case sf::Keyboard::Z: key_z = false; break;
        default: break;
    }
}

void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window->close();
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
        default: break;
    }
}

int main(int argc, char* argv[]) {
    window = new sf::RenderWindow(sf::VideoMode(RES_X, RES_Y), "Test Sfml Game");
    sf::Clock clock;

    float frameTime = 1/60.f;
    float dTime = 0;

    initialiseGame();

    while (window->isOpen()) {
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        // Event handling
        sf::Event event;
        while(window->pollEvent(event)) {
            processEvent(event);
        }

        // Safeguard (slowdown) to prevent game from lagging to death
        if (dTime > 5*frameTime) dTime = 5*frameTime;
        
        // Update game
        while (dTime > frameTime) {
            dTime -= frameTime;
            update();
        }

        // Draw frame
        window->clear();
        drawGameFrame();
        window->display();
    }

    return 0;
}
