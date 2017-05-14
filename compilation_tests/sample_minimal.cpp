#include <SFML/Graphics.hpp>
#include <iostream>

int RES_X = 800;
int RES_Y = 600;
sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "Sample Game");

int main() {
    std::cout << "Success";
}