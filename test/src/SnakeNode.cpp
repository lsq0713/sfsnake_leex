#include <SFML/Graphics.hpp>

#include "SnakeNode.h"
#include "Game.h"

using namespace sfSnake;

float SnakeNode::radius = 20.f;

SnakeNode::SnakeNode(sf::Vector2f position)
: position_(position)
{
	shape_.setPosition(position_);
	
	shape_.setRadius(radius * 50);

	setOriginMiddle(shape_);
}

SnakeNode::SnakeNode(sf::Vector2f position, Direction direction, float nodeRadius)
: position_(position), direction_(direction)
{
	radius = nodeRadius;// 加入对半径的初始化以统一程序
	shape_.setPosition(position_);

	shape_.setRadius(radius);

	setOriginMiddle(shape_);
}

void SnakeNode::setPosition(sf::Vector2f position)
{
	position_ = position;
	shape_.setPosition(position_);
}

void SnakeNode::setPosition(float x, float y)
{
	position_.x = x;
	position_.y = y;
	shape_.setPosition(position_);
}
void SnakeNode::setDirection(Direction direction)
{
	direction_ = direction;
}

void SnakeNode::setDirection(float x, float y)
{
	direction_ = Direction(x, y);
}

void SnakeNode::move(float xOffset, float yOffset)
{
	position_.x += xOffset;
	position_.y += yOffset;
	shape_.setPosition(position_);
}

sf::FloatRect SnakeNode::getBounds() const
{
	return shape_.getGlobalBounds();
}

sf::Vector2f SnakeNode::getPosition() const
{
	return position_;
}
Direction SnakeNode::getDirection() const
{
	return direction_;
}

void SnakeNode::render(sf::RenderWindow& window)
{
	// 节点的render貌似无法很好的绘制精灵，暂未找到原因，故弃用
	window.draw(shape_);
}