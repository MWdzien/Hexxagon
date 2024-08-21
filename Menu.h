#ifndef HEXXAGON_MENU_H
#define HEXXAGON_MENU_H

#include "WinScreen.h"
#include <map>

struct HighScore{
    std::string playerName;
    int points;

    bool operator<(const HighScore& otherPlayer) const {
        return points > otherPlayer.points;
    }
};

class Menu : public sf::Drawable{
private:
    void initMenu();

public:
    Menu();

    sf::Text title;
    std::array<sf::Text, 5> text;
    std::array<sf::Text, 5> highScoreText;
    std::array<sf::RectangleShape, 5> buttons;
    sf::RectangleShape border;

    sf::Text playerNameText;

    std::vector<HighScore> highestScore;

    void saveScore();
    void readScore();
    void hoverAnimations(sf::Vector2i mousePosition);

protected:
    void draw(sf::RenderTarget &target, const sf::RenderStates states) const override;
};


#endif
