#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;

Snake::Snake() : direction_(Direction(0, -1)), tailOverlap_(0u), score_(0), speedup_(false), nodeRadius_(Game::GameVideoMode.width / 100.0f), hitSelf_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/pickup.wav");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);

	// 头部节点的精灵初始化
	headTexture.loadFromFile("Image/headNode.png");
	headTexture.setSmooth(true);
	sf::Vector2u TextureSize = headTexture.getSize();
	float headScale = nodeRadius_ / TextureSize.y * 2.6;
	headSprite.setTexture(headTexture);
	headSprite.setScale(headScale, headScale);
	setOriginMiddle(headSprite);

	// 关节节点的精灵初始化
	nodeTexture.loadFromFile("Image/Node.png");
	nodeTexture.setSmooth(true);
	sf::Vector2u nodeTextureSize = nodeTexture.getSize();
	float nodeScale = nodeRadius_ / nodeTextureSize.y * 2.6;
	shape_.setTexture(nodeTexture);
	shape_.setScale(nodeScale, nodeScale);
	setOriginMiddle(shape_);
}

void Snake::initNodes()
{
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		nodes_.push_back(SnakeNode(sf::Vector2f(
			Game::GameVideoMode.width / 2 - nodeRadius_ / 2,
			Game::GameVideoMode.height / 2 - (nodeRadius_ / 2) + (nodeRadius_ * i * 2)), Direction(0, -1), nodeRadius_));
	}
}

void Snake::handleInput(sf::RenderWindow &window)
{
	// printf("Snake begin handleInput\n");
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		direction_ = Direction(0, -1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		direction_ = Direction(0, 1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		direction_ = Direction(-1, 0);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		direction_ = Direction(1, 0);

	static double directionSize;

	if (!Game::mouseButtonLocked)
	{
		if (
			sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
			sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			static sf::Vector2i MousePosition;
			MousePosition = sf::Mouse::getPosition(window);
			if (
				dis(MousePosition,
					sf::Vector2f(
						Game::GameVideoMode.width / 15.0f * 14.0f,
						Game::GameVideoMode.width / 15.0f)) >
				Game::GameVideoMode.width / 16.0f)
			{
				direction_ =
					static_cast<sf::Vector2f>(MousePosition) -
					toWindow(nodes_[0].getPosition());
				directionSize = length(direction_);
				direction_.x /= directionSize;
				direction_.y /= directionSize;
			}
		}
	}
}

void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit> &fruits)
{
	decltype(fruits.begin()) toRemove = fruits.end();

	for (auto it = fruits.begin(); it != fruits.end(); ++it)
	{
		if (it->getBounds().intersects(nodes_[0].getBounds()))
			toRemove = it;
	}

	if (toRemove != fruits.end())
	{
		pickupSound_.play();
		grow(toRemove->getScore());
		fruits.erase(toRemove);
	}
}

void Snake::grow(int score)
{
	tailOverlap_ += score * 3;
	score_ += score;
}

unsigned Snake::getSize() const
{
	return nodes_.size();
}

unsigned Snake::getScore() const
{
	return score_;
}

bool Snake::hitSelf() const
{
	return hitSelf_;
}

void Snake::checkSelfCollisions()
{
	SnakeNode &headNode = nodes_[0];
	if (nodes_.size() < 5)
	{
		dieSound_.play();
		sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
		hitSelf_ = true;
	} // 此处逻辑是为了后续加入减少长度机制设计的，同时增强健壮性，保证总有死亡可能
	for (decltype(nodes_.size()) i = 5; i < nodes_.size(); ++i)
	{
		if (headNode.getBounds().intersects(nodes_[i].getBounds()))
		{
			dieSound_.play();
			sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
			hitSelf_ = true;
		}
	}
}

void Snake::checkEdgeCollisions()
{
	SnakeNode &headNode = nodes_[0];

	if (headNode.getPosition().x <= 0)
		headNode.setPosition(Game::GameVideoMode.width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::GameVideoMode.width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::GameVideoMode.height);
	else if (headNode.getPosition().y >= Game::GameVideoMode.height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move()
{
	SnakeNode &headNode = nodes_[0];
	int times = speedup_ ? 2 : 1;
	for (int i = 1; i <= times; i++)
	{
		nodes_.insert(nodes_.begin(),
					  SnakeNode(sf::Vector2f(headNode.getPosition().x + direction_.x * i * nodeRadius_ * 2,
											 headNode.getPosition().y + direction_.y * i * nodeRadius_ * 2), direction_, nodeRadius_));
		if (tailOverlap_)
			tailOverlap_--;
		else
		{
			nodes_.pop_back();
		}
	}
}

void Snake::render(sf::RenderWindow &window)
{
	headSprite.setPosition(nodes_[0].getPosition());
	window.draw(headSprite);
	static float angle;
	static sf::Vector2f recDirection;
	recDirection = direction_;
	angle =
		std::acos(recDirection.y / length(recDirection)) /
		3.14159265358979323846 * 180.0;
	if (direction_.x > 0)
		angle = -angle;
	headSprite.setRotation(angle);

	for (int i = 1; i < nodes_.size(); i++)
	{
		shape_.setPosition(nodes_[i].getPosition());
		Direction idirection = nodes_[i].getDirection();
		recDirection = idirection;
		angle =
			std::acos(recDirection.y / length(recDirection)) /
			3.14159265358979323846 * 180.0;
		if (idirection.x > 0)
			angle = -angle;
		shape_.setRotation(angle);
		window.draw(shape_);
	}
}

sf::Vector2f Snake::toWindow(sf::Vector2f node)
{

	while (node.x < 0)
		node.x = node.x + Game::GameVideoMode.width;
	while (node.x > Game::GameVideoMode.width)
		node.x = node.x - Game::GameVideoMode.width;
	while (node.y < 0)
		node.y = node.y + Game::GameVideoMode.height;
	while (node.y > Game::GameVideoMode.height)
		node.y = node.y - Game::GameVideoMode.height;
	return node;
}