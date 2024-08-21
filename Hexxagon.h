#ifndef HEXXAGON_HEXXAGON_H
#define HEXXAGON_HEXXAGON_H

#include "Menu.h"

class Hexxagon : public sf::Drawable {
private:
    sf::Event event;

    Menu menu;
    Board board;
    WinScreen winScreen;



    Mode mode;
    WhichPlayer playerTurn;
    bool gameEnded;
    WhichPlayer winner;
    int player1PointCount;
    int player2PointCount;
    std::vector<Tile*> player1Tiles;
    std::vector<Tile*> player2Tiles;
    Tile* selectedTile;
    bool isSelected;
    std::vector<Tile*> possibleDupMoves;
    std::vector<Tile*> possibleLeapMoves;
    std::string playerName;

    sf::Text player1Score;
    sf::Text player2Score;


    void initWindow();
    void initData();
    void updateText();

    void buttonClicked(sf::Vector2i mousePosition);
    void backToMenuClicked(sf::Vector2i mousePosition);
    void selectTile(Tile* tile);
    Tile* whichTileSelected(sf::Vector2i mousePosition);
    std::array<std::vector<Tile*>, 2> calculatePossibleMoves(Tile* tile);
    void moveSelected(Tile* tile);
    void capture(Tile* tile);
    void deselect();
    void changeTurn();

    void occupiedStatus();
    void checkOccupied();
    void countPlayerTiles();

    void startPvsAI();
    void startPvsP();
    void showHighScore();
    void changePlayerName();

public:
    Hexxagon();

    sf::RenderWindow* window;
    Scene scene;

    bool running() const;
    void pollEvent();
    bool isAITurn();
    void checkWinner();
    void computerMove();

    void draw(sf::RenderTarget &target, const sf::RenderStates states) const override;
};


#endif
