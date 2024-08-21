/**
 * @file Tile.cpp
 * @brief Implementation of the Tile class.
*/
#include "Tile.h"

/**
 * @brief Default constructor for the Tile class.
*/
Tile::Tile() : hex(50, 6){
    initData();
}
/**
 * @brief Initializes the data of the tile, such as occupiedBy, playable, and hex properties.
*/
void Tile::initData() {
    font.loadFromFile("res/fonts/TwCenMTStd-SemiMedium.otf");
    this->occupiedBy = EMPTY;
    this->playable = false;
    hex.setRotation(90.f);
    hex.setOutlineThickness(2);
    hex.setOutlineColor(backgroundColor);
}
/**
 * @brief Returns the player who occupies the tile.
 * @return The player who occupies the tile.
*/
WhichPlayer Tile::getOccupiedBy() {
    return this->occupiedBy;
}
/**
 * @brief Sets the player who occupies the tile.
 * @param player The player who occupies the tile.
*/
void Tile::setOccupiedBy(WhichPlayer player) {
    this->occupiedBy = player;
}
/**
 * @brief Checks if the tile is playable.
 * @return True if the tile is playable, false otherwise.
*/
bool Tile::isPlayable() const {
    return this->playable;
}
/**
 * @brief Sets the playability of the tile.
 * @param playable The playability of the tile.
*/
void Tile::setPlayable(bool playable) {
    this->playable = playable;
}
/**
 * @brief Gets the coordinates of the tile.
 * @return The coordinates of the tile.
*/

std::pair<int, int> Tile::getCoords() {
    return this->coords;
}
/**
 * @brief Sets the coordinates of the tile.
 * @param coords The coordinates of the tile.
*/
void Tile::setCoords(std::pair<int, int> coords) {
    this->coords = coords;
}



