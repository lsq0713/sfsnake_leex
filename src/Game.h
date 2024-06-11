#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <memory>
#include <cmath>

#include "Screen.h"

namespace sfSnake
{
	class Game
	{
	public:
		Game();

		void run();

		void handleInput();
		void update(sf::Time delta);
		void render();

		static const int Width = 640;
		static const int Height = 480;

		static std::shared_ptr<Screen> GameScreen;

		static sf::VideoMode initVideoMode_(); // 初始化游戏初始窗口大小

		static sf::VideoMode GameVideoMode; // 游戏窗口大小参数

		static bool mouseButtonLocked;

	private:
		sf::RenderWindow window_;
		sf::Music bgMusic_;

		static const sf::Time TimePerFrame;
	};

	/**
	 * @brief 计算两个节点之间的距离
	 *
	 * @param node sf::Vector2
	 *
	 * @return double距离
	 */
	template <typename T1, typename T2>
	inline double dis(
		sf::Vector2<T1> node1,
		sf::Vector2<T2> node2) noexcept
	{
		return std::sqrt(
			std::pow(
				(
					static_cast<double>(node1.x) -
					static_cast<double>(node2.x)),
				2) +
			std::pow(
				(
					static_cast<double>(node1.y) -
					static_cast<double>(node2.y)),
				2));
	};
	/**
	 * @brief calculate the length of a vector
	 *
	 * @param node sf::Vector2
	 *
	 * @return double
	 */
	template <typename T>
	inline double length(sf::Vector2<T> node) noexcept
	{
		return std::sqrt(
			std::pow(static_cast<double>(node.x), 2) +
			std::pow(static_cast<double>(node.y), 2));
	};
}

#endif