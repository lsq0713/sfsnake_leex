#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "Game.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

using namespace sfSnake;

GameOverScreen::GameOverScreen(std::size_t score) : score_(score)
{
	font_.loadFromFile("Fonts/menu.ttf");
	text_.setFont(font_);
	text_.setString(
		sf::String(L"本轮得分: ") + std::to_wstring(score) +
		sf::String(L"\n\n[SPACE] 再来一局") +
		sf::String(L"\n\n[ESC]   不玩啦"));
	text_.setFillColor(sf::Color::Red);
	text_.setCharacterSize(64);

	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	text_.setPosition(Game::GameVideoMode.width / 2, Game::GameVideoMode.height / 2);
}

void GameOverScreen::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Game::GameScreen = std::make_shared<GameScreen>();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
}

void GameOverScreen::update(sf::Time delta)
{

}

void GameOverScreen::render(sf::RenderWindow& window)
{
	window.draw(text_);
}