/**
 * @file Menu.cpp
 * @brief Implementation of the Menu class.
*/
#include <fstream>
#include <iostream>
#include <sstream>
#include "Menu.h"

/**
 * @brief Default constructor for the Menu class.
*/
Menu::Menu() : text{sf::Text("", font), sf::Text("", font), sf::Text("", font), sf::Text("", font), sf::Text("", font)}, title("", font), playerNameText("player1", font, 48), highScoreText{sf::Text("", font), sf::Text("", font), sf::Text("", font), sf::Text("", font), sf::Text("", font)}{
    initMenu();
}
/**
 * @brief Initializes the menu by setting up buttons, text, and border.
*/
void Menu::initMenu() {
    for (int i = 0; i < 5; ++i) {
        this->buttons.at(i).setSize({500, 40});
        this->buttons.at(i).setPosition({390, 350+(float)(i*65)});
        this->buttons.at(i).setFillColor(sf::Color::Transparent);
        this->buttons.at(i).setOutlineColor(sf::Color::Transparent);
        this->buttons.at(i).setOutlineThickness(2);
    }

    title.setString("HEXXAGON");
    title.setCharacterSize(64);
    title.setPosition({640-(float)(title.getString().getSize()*19), 100});
    title.setFillColor(textColor);

    text.at(0).setString("Player vs AI");
    text.at(1).setString("Player vs Player");
    text.at(2).setString("Highest score");
    text.at(3).setString("Change name");
    text.at(4).setString("Exit game");

    for (int i = 0; i < 5; ++i) {
        this->text.at(i).setCharacterSize(32);
        this->text.at(i).setPosition({640 - this->text.at(i).getLocalBounds().width/2, 347+(float)(i*65)});
        this->text.at(i).setFillColor(textColor);
    }

    border.setSize({1210, 890});
    border.setPosition({35, 35});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(textColor);
    border.setOutlineThickness(3);

    playerNameText.setPosition({640 - playerNameText.getLocalBounds().width/2, 400});
    playerNameText.setFillColor(textColor);
}
/**
 * @brief Performs hover animations on the menu buttons based on the mouse position.
 * @param mousePosition The current position of the mouse.
*/
void Menu::hoverAnimations(sf::Vector2i mousePosition) {
    for (auto &button : buttons){
        if (button.getGlobalBounds().contains({static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)})){
            button.setOutlineColor(textColor);
        } else button.setOutlineColor(sf::Color::Transparent);

    }
}
/**
 * @brief Draws the menu objects on the render target.
 * @param target The render target to draw on.
 * @param states The render states to use for drawing.
*/
void Menu::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    target.draw(border);
    for (const auto& button : this->buttons) target.draw(button);
    for (const auto& text : this->text) target.draw(text);
    target.draw(title);
}
/**
 * @brief Saves five highest scores to the file
 */
void Menu::saveScore() {
    std::ofstream file("scores.txt");
    for (const auto& score : this->highestScore) {
        file << score.playerName << " " << score.points << std::endl;
    }
    file.close();
}
/**
 * @brief Reads five best scores from the file.
 */
void Menu::readScore() {
    std::ifstream file("scores.txt");
    if (!file.is_open()) {
        std::cout << "No file found." << std::endl;
        return;
    }
    highestScore.clear();
    std::string line;
    while (std::getline(file, line)){
        std::istringstream iss(line);
        HighScore hs;
        iss >> hs.playerName >> hs.points;
        highestScore.push_back(hs);
    }
    file.close();

    std::sort(highestScore.begin(), highestScore.end());

    if (highestScore.size() > 5) highestScore.resize(5);

}
