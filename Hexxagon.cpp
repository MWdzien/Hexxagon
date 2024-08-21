/**
 * @file Hexxagon.cpp
 * @brief Implementation file for the Hexxagon class.
 */
#include "Hexxagon.h"
#include "Tile.h"
#include <cmath>
#include <unistd.h>

/**
 * @brief Default constructor for the Hexxagon class.
 */
Hexxagon::Hexxagon() : menu(), winScreen(), player1Score("", font, 32), player2Score("", font, 32){
    initData();
    initWindow();
}
/**
 * @brief Initializes the game window.
 */
void Hexxagon::initWindow() {
    this->window = new sf::RenderWindow(sf::VideoMode({1280, 960}), "Hexxagon", sf::Style::Titlebar | sf::Style::Close);
    for (int i = 0; i < 5; ++i) {
        menu.text.at(i).setPosition({640 - menu.text.at(i).getLocalBounds().width/2, 347+(float)(i*65)});
    }
}
/**
 * @brief Initializes the game data.
 */
void Hexxagon::initData() {
    player1Score.setPosition({100, 850});
    player2Score.setPosition({1000, 850});
    player1Score.setFillColor(textColor);
    player2Score.setFillColor(textColor);

    playerName = "player1";

    for (auto &arr : board.tiles)
        for (auto &tile : arr)
            tile.initData();
    player1Tiles.clear();
    player2Tiles.clear();
    board.initBoard();
    mode = AI;
    playerTurn = PLAYER1;
    selectedTile = nullptr;
    isSelected = false;
    possibleDupMoves.clear();
    possibleLeapMoves.clear();
    player1PointCount = 0;
    player2PointCount = 0;
    gameEnded = false;
    winner = EMPTY;
    scene = MENU;
    occupiedStatus();
}
/**
 * @brief Checks if the game is running.
 * @return True if the game is running, false otherwise.
 */
bool Hexxagon::running() const {
    return window->isOpen();
}
/**
 * @brief Polls the events in the game window.
 * Depending on the current scene it show and polls either the menu, game board,
 * high score window and player name picker
 */
void Hexxagon::pollEvent() {
    while (this->window->pollEvent(this->event)) {
        if (event.type == sf::Event::Closed)
            window->close();

        switch (scene) {
            case MENU:
                if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    menu.hoverAnimations(mousePosition);
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    buttonClicked(mousePosition);
                }
                break;
            case GAME:
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    if (!isSelected)
                        selectTile(whichTileSelected(mousePosition));
                    else {
                        if (std::count(possibleDupMoves.begin(), possibleDupMoves.end(),
                                       whichTileSelected(mousePosition)) ||
                            std::count(possibleLeapMoves.begin(), possibleLeapMoves.end(),
                                       whichTileSelected(mousePosition))) {
                            moveSelected(whichTileSelected(mousePosition));
                            checkWinner();
                        } else deselect();
                    }
                }
                if (gameEnded)
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                        backToMenuClicked(mousePosition);
                    }
                updateText();
                board.repaintTiles();
                break;
            case HIGHSCORE:
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    scene = MENU;
                break;
            case PLAYERNAME:
                menu.playerNameText.setPosition({640 - menu.playerNameText.getLocalBounds().width/2, 400});
                menu.playerNameText.setFillColor(textColor);
                menu.playerNameText.setString(playerName);
                if (event.type == sf::Event::TextEntered)
                    if (event.text.unicode < 128)
                        if (event.text.unicode == '\b' && !playerName.empty())
                            playerName.pop_back();
                        else if (event.text.unicode == 13)
                            scene = MENU;
                        else if (event.text.unicode != '\b')
                            playerName += static_cast<char>(event.text.unicode);
                break;
        }
    }
}
/**
 * @brief Checks if it's AI mode
 * and calls the appropriate function accordingly.
*/
bool Hexxagon::isAITurn(){
    if (mode == AI && playerTurn == PLAYER2) return true;
    return false;
}
/**
 * @brief Updates the text for score counter.
 */
void Hexxagon::updateText() {
    countPlayerTiles();
    player1Score.setString(playerName + " score: " + std::to_string(player1PointCount));
    player2Score.setString("BLUE score: " + std::to_string(player2PointCount));
}
/**
 * @brief Checks the winner of the match and, if the match is over, launches the Win Screen
 */
void Hexxagon::checkWinner() {
    bool isBoardFilled = true;
    bool player1NoMovesLeft = false;
    bool player2NoMovesLeft = false;

    for (const auto& arr : board.tiles)
        for (auto tile : arr)
            if (tile.isPlayable() && tile.getOccupiedBy() == EMPTY) isBoardFilled = false;

    if (player1Tiles.empty()) player1NoMovesLeft = true;
    if (player2Tiles.empty()) player2NoMovesLeft = true;

    for (auto tile : player1Tiles){
        std::array<std::vector<Tile *>, 2> availableTiles = calculatePossibleMoves(tile);

        if (!(availableTiles.at(0).empty()) || !(availableTiles.at(1).empty())){
            player1NoMovesLeft = false;
            break;
        }
        player1NoMovesLeft = true;
    }

    for (auto tile : player2Tiles){
        std::array<std::vector<Tile *>, 2> availableTiles = calculatePossibleMoves(tile);

        if (!(availableTiles.at(0).empty()) || !(availableTiles.at(1).empty())){
            player2NoMovesLeft = false;
            break;
        }
        player2NoMovesLeft = true;
    }

    if (player1NoMovesLeft) player2PointCount = 58 - player1PointCount;
    if (player2NoMovesLeft) player1PointCount = 58 - player2PointCount;

    if (isBoardFilled || player1NoMovesLeft || player2NoMovesLeft) {
        gameEnded = true;
        if (player1PointCount > player2PointCount) winner = PLAYER1;
        else if (player2PointCount > player1PointCount) winner = PLAYER2;
        else winner = EMPTY;


        winScreen.initWinScreen(winner);
    }
}
/**
 * @brief Makes a move for the computer calculating the best possible outcome.
 */
void Hexxagon::computerMove() {
    sleep(1);

    Tile *bestTile;
    Tile *bestTargetTile;
    int mostPossibleMoves = 0;
    int bestCaptureCounter = 0;

    for (auto tile : player2Tiles){
        std::array<std::vector<Tile *>, 2> availableTiles = calculatePossibleMoves(tile);

        if (availableTiles.at(0).size() + availableTiles.at(1).size() > mostPossibleMoves)
            bestTile = tile;
        mostPossibleMoves = availableTiles.at(0).size() + availableTiles.at(1).size();
    }

    for (const auto& arr : calculatePossibleMoves(bestTile))
        for (auto tmpBestTarget : arr){
            bestTargetTile = tmpBestTarget;
        }

    for (auto tile: player2Tiles) {
        std::array<std::vector<Tile *>, 2> availableTiles = calculatePossibleMoves(tile);

        for (int i = 0; i < availableTiles.size(); i++) {
            for (auto targetTile : availableTiles.at(i)) {
                int captureCounter = 0;
                if (!i) captureCounter++;
                for (auto &arr: board.tiles)
                    for (auto &t: arr) {
                        if (abs(targetTile->hex.getPosition().x - t.hex.getPosition().x) <= 100 &&
                            abs(targetTile->hex.getPosition().y - t.hex.getPosition().y) <= 100)
                            if (t.getOccupiedBy() == PLAYER1) captureCounter++;
                    }
                if (captureCounter > bestCaptureCounter) {
                    bestTile = tile;
                    bestTargetTile = targetTile;
                    bestCaptureCounter = captureCounter;
                }
            }
        }
    }
    selectTile(bestTile);
    moveSelected(bestTargetTile);
    checkWinner();
}
/**
 * @brief Checks which button from the main menu has been clicked.
 * @param mousePosition The position of the mouse click.
 */
void Hexxagon::buttonClicked(sf::Vector2i mousePosition) {
    int clickedIndex = 0;
    for (int i = 0; i < 5; ++i) {
        if (menu.buttons.at(i).getGlobalBounds().contains({static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)}))
            clickedIndex = i;
    }
    switch (clickedIndex){
        case 0:
            startPvsAI();
            break;
        case 1:
            startPvsP();
            break;
        case 2:
            showHighScore();
            break;
        case 3:
            changePlayerName();
            break;
        case 4:
            window->close();
    }
}
/**
 * @brief Handles the event when the "Back to Menu" button is clicked.
 * @param mousePosition The position of the mouse click.
*/
void Hexxagon::backToMenuClicked(sf::Vector2i mousePosition) {
    if (winScreen.backToMenuButton.getGlobalBounds().contains({static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)})){
        menu.highestScore.push_back({playerName, player1PointCount});
        menu.saveScore();
        initData();
    }
}
/**
 * @brief Selects a tile on the board.
 * @param tile Pointer to the selected tile.
*/
void Hexxagon::selectTile(Tile *tile) {
    if (tile == nullptr) return;

    if (tile->getOccupiedBy() == EMPTY) return;
    if (playerTurn == PLAYER1 && tile->getOccupiedBy() == PLAYER2) return;
    if (playerTurn == PLAYER2 && tile->getOccupiedBy() == PLAYER1) return;

    this->selectedTile = tile;
    this->isSelected = true;

    possibleDupMoves = calculatePossibleMoves(tile)[0];
    possibleLeapMoves = calculatePossibleMoves(tile)[1];

    if (possibleDupMoves.empty() && possibleLeapMoves.empty()) return;

    for (auto &e: possibleDupMoves) {
        e->hex.setOutlineColor(sf::Color::Green);
    }
    for (auto &e: possibleLeapMoves) {
        e->hex.setOutlineColor(sf::Color::Yellow);
    }
}
/**
 * @brief Determines which tile is selected based on the mouse position.
 * @param mousePosition The position of the mouse click.
 * @return Pointer to the selected tile, or nullptr if no tile is selected.
*/
Tile *Hexxagon::whichTileSelected(sf::Vector2i mousePosition) {
    for (auto &arr : board.tiles)
        for (auto &tile : arr)
            if (tile.hex.getGlobalBounds().contains({static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)}))
                return &tile;
    return nullptr;
}
/**
 * @brief Calculates the possible duplicate and leap moves from a given tile.
 * @param tile Pointer to the tile for which to calculate the possible moves.
 * @return An array containing two vectors of Tile pointers. The first vector represents the possible duplicate moves,
 * and the second vector represents the possible leap moves.
*/
std::array<std::vector<Tile *>, 2> Hexxagon::calculatePossibleMoves(Tile *tile) {
    std::array<std::vector<Tile*>, 2> availableTiles;

    for (auto &arr : board.tiles)
        for (auto &targetTile : arr){
            if (targetTile.getCoords() == tile->getCoords()) continue;
            if (!targetTile.isPlayable()) continue;
            if (targetTile.getOccupiedBy() == PLAYER1 || targetTile.getOccupiedBy() == PLAYER2) continue;
            if (sqrt(pow(tile->hex.getPosition().x - targetTile.hex.getPosition().x, 2) + (pow(tile->hex.getPosition().y - targetTile.hex.getPosition().y, 2))) <= 250)
                if (sqrt(pow(tile->hex.getPosition().x - targetTile.hex.getPosition().x, 2) + (pow(tile->hex.getPosition().y - targetTile.hex.getPosition().y, 2))) <= 150)
                    availableTiles[0].push_back(&targetTile);
                else availableTiles[1].push_back(&targetTile);
        }

    return availableTiles;
}
/**
 * @brief Moves the selected tile to the specified tile.
 * @param tile Pointer to the tile to which the selected tile is moved.
*/
void Hexxagon::moveSelected(Tile* tile){
    if (!isSelected) return;

    if (std::count(possibleDupMoves.begin(), possibleDupMoves.end(), tile)){
        tile->setOccupiedBy(playerTurn);
    }

    if (std::count(possibleLeapMoves.begin(), possibleLeapMoves.end(), tile)){
        tile->setOccupiedBy(playerTurn);
        selectedTile->setOccupiedBy(EMPTY);
    }
    occupiedStatus();

    capture(tile);

    deselect();

    changeTurn();
}
/**
 * @brief Captures neighboring tiles if the conditions are met.
 * @param tile Pointer to the tile from which the capture is checked.
*/
void Hexxagon::capture(Tile *tile) {
    for (auto &arr : board.tiles)
        for (auto &t : arr){
            if (abs(tile->hex.getPosition().x - t.hex.getPosition().x) <= 100 &&
                abs(tile->hex.getPosition().y - t.hex.getPosition().y) <= 100){
                if (playerTurn == PLAYER1)
                    if (t.getOccupiedBy() == PLAYER2) {
                        t.setOccupiedBy(PLAYER1);
                        checkOccupied();
                    }

                if (playerTurn == PLAYER2)
                    if (t.getOccupiedBy() == PLAYER1) {
                        t.setOccupiedBy(PLAYER2);
                        checkOccupied();
                    }
            }
        }

    occupiedStatus();
}
/**
 * @brief Deselects the selected tile and clears the possible moves.
*/
void Hexxagon::deselect() {
    this->selectedTile = nullptr;
    this->isSelected = false;
    for (auto e : possibleDupMoves) {
        e->hex.setOutlineColor(backgroundColor);
    }
    for (auto e : possibleLeapMoves) {
        e->hex.setOutlineColor(backgroundColor);
    }
    possibleDupMoves.clear();
    possibleLeapMoves.clear();
}
/**
 * @brief Changes the turn from one player to the other.
*/
void Hexxagon::changeTurn() {
    if (playerTurn == PLAYER1)
        playerTurn = PLAYER2;
    else playerTurn = PLAYER1;
}
/**
 * @brief Updates the occupied status of the tiles and the player's tile lists.
*/
void Hexxagon::occupiedStatus() {
    for (auto& arr : board.tiles)
        for (auto &tile : arr){
            if (tile.getOccupiedBy() == PLAYER1 && !std::count(player1Tiles.begin(), player1Tiles.end(), &tile))
                player1Tiles.push_back(&tile);
            if (tile.getOccupiedBy() == PLAYER2 && !std::count(player2Tiles.begin(), player2Tiles.end(), &tile))
                player2Tiles.push_back(&tile);
            if (tile.getOccupiedBy() == EMPTY) {
                if (std::count(player1Tiles.begin(), player1Tiles.end(), &tile))
                    std::erase(player1Tiles, &tile);
                if (std::count(player2Tiles.begin(), player2Tiles.end(), &tile))
                    std::erase(player2Tiles, &tile);
            }
        }
    checkOccupied();
}
/**
 * @brief Checks and removes any invalid tiles from the player's tile lists.
*/
void Hexxagon::checkOccupied() {
    for (auto tile : player1Tiles){
        if (tile->getOccupiedBy() == EMPTY || tile->getOccupiedBy() == PLAYER2)
            std::erase(player1Tiles, tile);
    }
    for (auto tile : player2Tiles){
        if (tile->getOccupiedBy() == EMPTY || tile->getOccupiedBy() == PLAYER1)
            std::erase(player2Tiles, tile);
    }
}
/**
 * @brief Counts the number of tiles occupied by each player.
*/
void Hexxagon::countPlayerTiles() {
    int tmp1Count = 0;
    int tmp2Count = 0;
    for (const auto& arr : board.tiles)
        for (auto tile : arr){
            if (tile.getOccupiedBy() == PLAYER1) tmp1Count++;
            if (tile.getOccupiedBy() == PLAYER2) tmp2Count++;
        }
    player1PointCount = tmp1Count;
    player2PointCount = tmp2Count;
}
/**
 * @brief Starts a player versus AI game.
*/
void Hexxagon::startPvsAI() {
    scene = GAME;
    playerTurn = PLAYER1;
    mode = AI;
    occupiedStatus();
    gameEnded = false;
}
/**
 * @brief Starts a player versus player game.
*/
void Hexxagon::startPvsP(){
    scene = GAME;
    playerTurn = PLAYER1;
    mode = PVP;
    occupiedStatus();
    gameEnded = false;
}
/**
 * @brief Shows the high score screen.
*/
void Hexxagon::showHighScore() {
    scene = HIGHSCORE;
    menu.readScore();
    int i = 0;
    for (const auto& score : menu.highestScore){
        menu.highScoreText.at(i).setString(score.playerName + ": " + std::to_string(score.points));
        menu.highScoreText.at(i).setCharacterSize(32);
        menu.highScoreText.at(i).setPosition({640 - menu.highScoreText.at(i).getLocalBounds().width/2, 347+(float)(i*65)});
        menu.highScoreText.at(i).setFillColor(textColor);
        i++;
    }
}
/**
 * @brief Draws the game objects on the render target.
 * @param target The render target to draw on.
 * @param states The render states to use for drawing.
*/
void Hexxagon::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    switch (scene) {
        case MENU:
            target.draw(menu);
            break;
        case GAME:
            target.draw(board);
            target.draw(player1Score);
            target.draw(player2Score);
            if (gameEnded)
                target.draw(winScreen);
            break;
        case HIGHSCORE:
            target.draw(menu.border);
            for (const auto& el : menu.highScoreText)
                target.draw(el);
            break;
        case PLAYERNAME:
            target.draw(menu.border);
            target.draw(menu.playerNameText);
            break;

    }
}
/**
 * @brief Enters the player name change scene
 */
void Hexxagon::changePlayerName() {
    scene = PLAYERNAME;
}

