// v0 - Display
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// GLOBALS - START
int RES_X = 800;
int RES_Y = 600;
sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "This is not a game. This is serious business.");
// GLOBALS - END

void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window.close();
            break;
        }
    }
}

int main() {
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
    }

    return 0;
}