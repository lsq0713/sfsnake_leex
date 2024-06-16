#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// #include <vector>
#include <deque>

#include "SnakeNode.h"
#include "Fruit.h"

namespace sfSnake
{
	class Snake
	{
	public:
		Snake();

		void handleInput(sf::RenderWindow &window);
		void update(sf::Time delta);
		void render(sf::RenderWindow &window);

		void checkFruitCollisions(std::vector<Fruit> &fruits);

		bool hitSelf() const;

		unsigned getSize() const;
		unsigned getScore() const;

		sf::Vector2f toWindow(sf::Vector2f node); // 返回蛇头位置

	private:
		void move();
		void grow(int score);
		void checkEdgeCollisions();
		void checkSelfCollisions();
		void initNodes();

		bool hitSelf_;

		sf::Vector2f position_;
		Direction direction_;

		sf::SoundBuffer pickupBuffer_;
		sf::Sound pickupSound_;

		sf::SoundBuffer dieBuffer_;
		sf::Sound dieSound_;

		std::deque<SnakeNode> nodes_;

		static const int InitialSize;

		unsigned tailOverlap_; // 尾部重叠数
		unsigned int score_;   // 增长分数
		bool speedup_;		   // 是否加速
		float nodeRadius_;	   // 节点半径

		sf::Texture headTexture;
		sf::Sprite headSprite;

		sf::Texture nodeTexture;
		sf::Sprite shape_;
	};
}

#endif