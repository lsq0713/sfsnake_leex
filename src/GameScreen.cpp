#include <SFML/Graphics.hpp>

#include <random>
#include <memory>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen() : snake_(), grid_()
{
	// score_.setFont(Game::GameFont);
	// score_.setString(sf::String(L"分数:") + std::to_string(snake_.getScore()));
	// score_.setFillColor(sf::Color::Blue);
	// setOriginMiddle(score_);
	// score_.setPosition(420.f, 30.f);
}

void GameScreen::handleInput(sf::RenderWindow &window)
{
	snake_.handleInput(window);
}

void GameScreen::update(sf::Time delta)
{
	if (fruit_.size() < 6)
		generateFruit();

	snake_.update(delta);
	snake_.checkFruitCollisions(fruit_);

	if (snake_.hitSelf())
	{
		Game::GameScreen = std::make_shared<GameOverScreen>(snake_.getSize());
	}
	// score_.setString(sf::String(L"分数:\t") + std::to_string(snake_.getScore()));
}

void GameScreen::render(sf::RenderWindow &window)
{
	if (Game::GridVisibility)
        grid_.render(window);
	snake_.render(window);

	for (auto fruit : fruit_)
		fruit.render(window);
	// window.draw(score_);
}

void GameScreen::generateFruit()
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> xDistribution(0, Game::GameVideoMode.width - SnakeNode::radius);
	static std::uniform_int_distribution<int> yDistribution(0, Game::GameVideoMode.height - SnakeNode::radius);
	static std::uniform_int_distribution<int> colorDistribution(0, 8);

	fruit_.push_back(Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)), colorDistribution(engine)));
}
