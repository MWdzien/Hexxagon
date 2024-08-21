/**
 * @file Board.cpp
 * @brief Implementation of the Board class.
 */

#include "Board.h"
/**
 * @brief Constructs a Board object.
 */
Board::Board(){
    initBoard();
}
/**
 * @brief Initializes the board.
 *
 * This function sets up the initial configuration of the board, including
 * the positions of player 1 and player 2, and determines the playable tiles.
 */
void Board::initBoard() {
    // Set occupied positions for player 1
    tiles.at(8).at(4).setOccupiedBy(PLAYER1);
    tiles.at(2).at(8).setOccupiedBy(PLAYER1);
    tiles.at(2).at(0).setOccupiedBy(PLAYER1);

    // Set occupied positions for player 2
    tiles.at(6).at(0).setOccupiedBy(PLAYER2);
    tiles.at(6).at(8).setOccupiedBy(PLAYER2);
    tiles.at(0).at(4).setOccupiedBy(PLAYER2);

    // Set coordinates and positions for all tiles
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            this->tiles.at(i).at(j).setCoords({i, j});
            this->tiles.at(i).at(j).hex.setPosition({290 + j * 100.f, 30 + (j % 2 ? 50 : 0) + i * 100.f});

            // Set playable tiles
            if (!((i == 0 && (j < 3 || j > 5))
                  || (i == 1 && (j == 0 || j == 8))
                  || (i == 3 && j == 4) || (i == 4 && (j == 3 || j == 5)
                  || (i == 7 && (j < 2 || j > 6))
                  || (i == 8 && j != 4))))
                tiles.at(i).at(j).setPlayable(true);
        }
    }
    // Change the colors of the tiles
    repaintTiles();
}
/**
 * @brief Repaints the tiles on the board.
 *
 * This function updates the fill color of the tiles based on the player occupying them.
 */
void Board::repaintTiles() {
    for (auto& e : tiles)
        for (auto &t : e)
            if (t.getOccupiedBy() == PLAYER1) t.hex.setFillColor(player1Color);
            else if (t.getOccupiedBy() == PLAYER2) t.hex.setFillColor(player2Color);
            else t.hex.setFillColor(hexColor);
}
/**
 * @brief Draws the board on the given render target.
 * @param target The target to draw the board on.
 * @param states The render states to use for drawing.
 */
void Board::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    for (const auto& arr : tiles)
        for (const auto& tile : arr)
            if(tile.isPlayable()) target.draw(tile.hex);
}