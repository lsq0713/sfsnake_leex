#include <SFML/Graphics.hpp>

#include <memory>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

MenuScreen::MenuScreen()
{
	// Game::GameFont.loadFromFile("Fonts/game.ttf");
	font_.loadFromFile("Fonts/game.ttf");
	text_.setFont(font_);
	text_.setString(
		sf::String(L"\n\n\n\n\n\n开始游戏：[空格]") +
		sf::String(L"\n\n退出游戏：[ESC]") +
		sf::String(L"\n\n开关/切换网格（无）：[G]") +
		sf::String(L"\n\n背景颜色：[B]"));
	text_.setCharacterSize(32);
	text_.setFillColor(sf::Color(0xA9A4B8ee));
	snakeText_.setFont(font_);
	snakeText_.setString(sf::String(L"贪吃蛇大作战！"));
	snakeText_.setFillColor(sf::Color::Green);
	snakeText_.setCharacterSize(64);
	snakeText_.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds = text_.getLocalBounds();
	// text_.setOrigin(textBounds.left + textBounds.width / 2,
	// 				textBounds.top + textBounds.height / 2);
	// text_.setPosition(Game::GameVideoMode.width / 2, Game::GameVideoMode.height / 2);
	text_.setPosition(180.f, 0.f);

	sf::FloatRect snakeTextBounds = snakeText_.getLocalBounds();
	// snakeText_.setOrigin(snakeTextBounds.left + snakeTextBounds.width / 2,
	// 					 snakeTextBounds.top + snakeTextBounds.height / 2);
	// snakeText_.setPosition(Game::GameVideoMode.width / 2, Game::GameVideoMode.height / 4);
	snakeText_.setPosition(280.f, 80.f);
}

void MenuScreen::handleInput(sf::RenderWindow &window)
{
	if (Game::keyButtonLocked)
	{
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Game::GameScreen = std::make_shared<GameScreen>();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
	{
		Game::keyButtonLocked = true;
		// 切换网格状态
		if (!Game::GridVisibility)
		{
			text_.setString(
				sf::String(L"\n\n\n\n\n\n开始游戏：[空格]") +
				sf::String(L"\n\n退出游戏：[ESC]") +
				sf::String(L"\n\n开关/切换网格（ 白色 ）：[G]") +
				sf::String(L"\n\n背景颜色：[B]"));
			Game::GridVisibility = true;
			Game::GridColor = 0;
		}
		else if (Game::GridColor == 0)
		{
			text_.setString(
				sf::String(L"\n\n\n\n\n\n开始游戏：[空格]") +
				sf::String(L"\n\n退出游戏：[ESC]") +
				sf::String(L"\n\n开关/切换网格（ 黑色 ）：[G]") +
				sf::String(L"\n\n背景颜色：[B]"));
			Game::GridColor++;
		}
		else if (Game::GridColor == 1)
		{
			text_.setString(
				sf::String(L"\n\n\n\n\n\n开始游戏：[空格]") +
				sf::String(L"\n\n退出游戏：[ESC]") +
				sf::String(L"\n\n开关/切换网格（ 棕色 ）：[G]") +
				sf::String(L"\n\n背景颜色：[B]"));
			Game::GridColor++;
		}
		else
		{
			text_.setString(
				sf::String(L"\n\n\n\n\n\n开始游戏：[空格]") +
				sf::String(L"\n\n退出游戏：[ESC]") +
				sf::String(L"\n\n开关/切换网格（无）：[G]") +
				sf::String(L"\n\n背景颜色：[B]"));
			Game::GridVisibility = false;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		Game::keyButtonLocked = true;
		Game::BgColor = (Game::BgColor + 1) % 3;
	}
}

void MenuScreen::update(sf::Time delta)
{
	static bool movingLeft = false;
	static bool movingRight = true;

	if (movingRight)
	{
		snakeText_.rotate(delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == 10)
		{
			movingRight = false;
			movingLeft = true;
		}
	}

	if (movingLeft)
	{
		snakeText_.rotate(-delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == (360 - 10))
		{
			movingLeft = false;
			movingRight = true;
		}
	}
}

void MenuScreen::render(sf::RenderWindow &window)
{
	window.draw(text_);
	window.draw(snakeText_);
}