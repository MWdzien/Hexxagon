#ifndef HEXXAGON_BOARD_H
#define HEXXAGON_BOARD_H

#include <array>
#include "Tile.h"


class Board : public sf::Drawable{
private:
    void draw(sf::RenderTarget &target, const sf::RenderStates states) const;
public:
    Board();
    void initBoard();

    std::array<std::array<Tile, 9>, 9> tiles;

    void repaintTiles();
};

#endif
