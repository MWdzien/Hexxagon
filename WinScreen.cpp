/**
 * @file WinScreen.cpp
 * @brief Implementation of the WinScreen class.
*/
#include "WinScreen.h"


/**
 * @brief Default constructor for the WinScreen class.
*/
WinScreen::WinScreen() : backToMenuText("Back to menu", font, 48), winText("", font, 128), winTextShadow("", font, 128){

}
/**
 * @brief Initializes the win screen with the winner information.
 * @param winner The player who won the game.
*/
void WinScreen::initWinScreen(WhichPlayer winner) {
    winWindow.setSize({1280, 480});
    winWindow.setPosition({0, 240});
    winWindow.setFillColor(sf::Color(0, 0, 0, 128));

    winText.setFillColor(sf::Color::White);
    winTextShadow.setFillColor(textColor);
    winText.setPosition({260, 400});
    winTextShadow.setPosition({260 + 3, 400 + 3});

    if (winner == PLAYER1) {
        winText.setString("PLAYER1 WINS!");
        winTextShadow.setString("PLAYER1 WINS!");
    }
    else if (winner == PLAYER2) {
        winTextShadow.setString("PLAYER2 WINS!");
        winText.setString("PLAYER2 WINS!");
    }
    else {
        winTextShadow.setString("DRAW!");
        winText.setString("DRAW!");
    }

    backToMenuButton.setSize({300, 70});
    backToMenuButton.setPosition({490,770});
    backToMenuButton.setFillColor(textColor);
    backToMenuText.setPosition({520, 770});
    backToMenuText.setFillColor(sf::Color::White);
}
/**
 * @brief Draws the win screen elements on the render target.
 * @param target The render target to draw on.
 * @param states The render states to apply.
*/
void WinScreen::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    target.draw(winWindow);
    target.draw(winTextShadow);
    target.draw(winText);
    target.draw(backToMenuButton);
    target.draw(backToMenuText);
}