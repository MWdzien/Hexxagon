#ifndef HEXXAGON_TILE_H
#define HEXXAGON_TILE_H

#include "EnumsAndGlobals.h"

class Tile {
private:
    bool playable;
    WhichPlayer occupiedBy;
    std::pair<int, int> coords;
public:
    Tile();
    void initData();

    sf::CircleShape hex;

    WhichPlayer getOccupiedBy();
    void setOccupiedBy(WhichPlayer player);
    bool isPlayable() const;
    void setPlayable(bool playable);
    std::pair<int, int> getCoords();
    void setCoords(std::pair<int, int> coords);

};


#endif
