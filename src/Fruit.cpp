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