#ifndef HEXXAGON_ENUMSANDGLOBALS_H
#define HEXXAGON_ENUMSANDGLOBALS_H

#include <SFML/Graphics.hpp>

enum WhichPlayer{
    EMPTY,
    PLAYER1,
    PLAYER2
};

enum Mode{
    AI,
    PVP
};

enum Scene{
    MENU,
    GAME,
    HIGHSCORE,
    PLAYERNAME
};

extern sf::Font font;
extern sf::Color backgroundColor;
extern sf::Color hexColor;
extern sf::Color player1Color;
extern sf::Color player2Color;
extern sf::Color textColor;

#endif
