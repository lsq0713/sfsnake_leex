#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
	typedef sf::Vector2f Direction;
	typedef sf::Vector2f Node;
	class SnakeNode
	{
	public:
		SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0));
		SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0), Direction direction = Direction(0, -1), float nodeRadius = 20.0f);

		void setPosition(sf::Vector2f position);
		void setPosition(float x, float y);

		void setDirection(Direction direction);
		void setDirection(float x, float y);

		void move(float xOffset, float yOffset);

		void render(sf::RenderWindow &window);

		sf::Vector2f getPosition() const;
		sf::FloatRect getBounds() const;
		Direction getDirection() const;

		static float radius;

	private:
		// sf::RectangleShape shape_;
		sf::Texture nodeTexture;
		// sf::Sprite shape_;
		sf::CircleShape shape_;
		sf::Vector2f position_;
		Direction direction_;
	};
}

#endif