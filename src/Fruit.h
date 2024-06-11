#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
class Fruit
{
public:
	Fruit(sf::Vector2f position = sf::Vector2f(0, 0));
	Fruit(sf::Vector2f position = sf::Vector2f(0, 0), int FruitType = 0);

	void render(sf::RenderWindow& window);

	sf::FloatRect getBounds() const;
	int getScore();

private:
	sf::CircleShape shape_;

	static const float Radius;

	int score_;
};
}

#endif