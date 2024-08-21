/**
 * @file main.cpp
 * @brief Implementation of the Hexxagon game.
*/
#include "Hexxagon.h"

/**
 * @brief Main function of the Hexxagon game.
 * @return The exit status of the program.
*/
int main() {
    Hexxagon hexxagon;

    while (hexxagon.running()){
        hexxagon.window->clear(sf::Color(242, 243, 217));
        if (hexxagon.isAITurn()) hexxagon.computerMove();
        else hexxagon.pollEvent();
        hexxagon.window->draw(hexxagon);
        hexxagon.window->display();
    }
}
