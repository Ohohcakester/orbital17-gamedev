// v4 - Gravity and Jumping
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Player;
void resetKeyClicks();

// GLOBALS - START
int RES_X = 800;
int RES_Y = 600;
sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "This is not a game. This is serious business.");

bool key_left = false;
bool key_right = false;
bool key_space_click = false;

Player* player;
// GLOBALS - END

class Player {
public:
    float x, y;
    float vx, vy;
    bool isOnGround;

    int width = 25;
    int height = 40;
    int speed = 3;
    float gravity = 0.4f;
    float jumpSpeed = 10;

    Player(float x, float y): x(x), y(y) {
        vx = 0;
        vy = 0;
        isOnGround = false;
    }

    void update() {
        vx = 0;
        vy += gravity;
        if (key_left) vx -= speed;
        if (key_right) vx += speed;
        if (key_space_click) {
            if (isOnGround) vy = -jumpSpeed;
        }
        
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

    void draw() {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Blue);
        shape.setSize(sf::Vector2f(width, height));
        shape.setOrigin(width/2, height/2);
        shape.setPosition(x, y);
        window.draw(shape);
    }
};

void initialiseGame() {
    if (player != nullptr) delete player;
    player = new Player(RES_X/2, RES_Y/4);
}

void updateGame() {
    player->update();

    resetKeyClicks();
}

void drawGameFrame() {
    player->draw();
}

void keyDown(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::Left: key_left = true; break;
        case sf::Keyboard::Right: key_right = true; break;
        case sf::Keyboard::Space: key_space_click = true; break;
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