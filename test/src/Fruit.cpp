#include <SFML/Graphics.hpp>

#include "Fruit.h"

using namespace sfSnake;

const float Fruit::Radius = 5.f;

Fruit::Fruit(sf::Vector2f position): score_(3)
{
	shape_.setPosition(position);
	shape_.setRadius(Fruit::Radius);
	shape_.setFillColor(sf::Color::Red);
}
Fruit::Fruit(sf::Vector2f position, int FruitType)
{
	shape_.setPosition(position);
	shape_.setRadius(Fruit::Radius);
	if (FruitType < 0)
	{
		FruitType = -FruitType;
	}
	FruitType = FruitType % 8;
	if (FruitType < 3)
	{
		shape_.setFillColor(sf::Color::Green);	// Green
		score_ = 1;
	}
	else if (FruitType < 5)
	{
		shape_.setFillColor(sf::Color::Blue);	// Blue
		score_ = 2;
	}
	else if (FruitType < 6)
	{
		shape_.setFillColor(sf::Color::Red);	// Red
		score_ = 1;
	}
	else if (FruitType < 7)
	{
		shape_.setFillColor(sf::Color(0x935116ee));// Brown
		score_ = 0;
	}
	else
	{
		shape_.setFillColor(sf::Color(0xe00aeaee));// Black // TODO
		score_ = 0;
	}
	// printf("x: %f, y: %f, color: %d\n", position.x, position.y, FruitType);
}

void Fruit::render(sf::RenderWindow& window)
{
	window.draw(shape_);
}

sf::FloatRect Fruit::getBounds() const
{
	return shape_.getGlobalBounds();
}

int Fruit::getScore()
{
	return score_;
}