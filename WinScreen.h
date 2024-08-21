#ifndef HEXXAGON_WINSCREEN_H
#define HEXXAGON_WINSCREEN_H

#include "Board.h"

class WinScreen : public sf::Drawable{
private:
    sf::RectangleShape winWindow;
    sf::Text winText;
    sf::Text winTextShadow;
    sf::Text backToMenuText;

public:
    sf::RectangleShape backToMenuButton;
    WinScreen();
    void initWinScreen(WhichPlayer winner);

protected:
    void draw(sf::RenderTarget &target, const sf::RenderStates states) const override;
};


#endif
