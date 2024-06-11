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

Snake::Snake() : direction_(Direction(0, -1)), tailOverlap_(0u), score_(InitialSize), speedup_(false), nodeRadius_(Game::GameVideoMode.width / 100.0f), hitSelf_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/pickup.wav");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);
}

void Snake::initNodes()
{
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		nodes_.push_back(SnakeNode(sf::Vector2f(
			Game::Width / 2 - SnakeNode::Width / 2,
			Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i))));
	}
}

void Snake::handleInput(sf::RenderWindow &window)
{
	printf("Snake begin handleInput\n");
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

/*void Snake::grow()
{
	switch (direction_)
	{
	case Direction::Up:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
			nodes_[nodes_.size() - 1].getPosition().y + SnakeNode::Height)));
		break;
	case Direction::Down:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
			nodes_[nodes_.size() - 1].getPosition().y - SnakeNode::Height)));
		break;
	case Direction::Left:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + SnakeNode::Width,
			nodes_[nodes_.size() - 1].getPosition().y)));
		break;
	case Direction::Right:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x - SnakeNode::Width,
			nodes_[nodes_.size() - 1].getPosition().y)));
		break;
	}
}*/
void Snake::grow(int score)
{
	tailOverlap_ += score * 5;
	score_ += score;
}

unsigned Snake::getSize() const
{
	return nodes_.size();
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
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::Width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	else if (headNode.getPosition().y >= Game::Height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

/*void Snake::move()
{
	for (decltype(nodes_.size()) i = nodes_.size() - 1; i > 0; --i)
	{
		nodes_[i].setPosition(nodes_.at(i - 1).getPosition());
	}

	switch (direction_)
	{
	case Direction::Up:
		nodes_[0].move(0, -SnakeNode::Height);
		break;
	case Direction::Down:
		nodes_[0].move(0, SnakeNode::Height);
		break;
	case Direction::Left:
		nodes_[0].move(-SnakeNode::Width, 0);
		break;
	case Direction::Right:
		nodes_[0].move(SnakeNode::Width, 0);
		break;
	}
}*/
void Snake::move()
{
	SnakeNode &headNode = nodes_[0];
	// debug
	int debug = 0;
	for (auto node : nodes_)
	{
		std::cout << "node " << debug << " x: " << node.getPosition().x << " y: " << node.getPosition().y << std::endl;
		debug++;
	}
	// debug
	int times = speedup_ ? 2 : 1;
	for (int i = 1; i <= times; i++)
	{
		/*nodes_.insert(nodes_.begin(),
		SnakeNode(sf::Vector2f(headNode.getPosition().x + direction_.x * i * nodeRadius_ / 5.0,
			headNode.getPosition().y + direction_.y * i * nodeRadius_ / 5.0)
			));*/
		nodes_.insert(nodes_.begin(),
					  SnakeNode(sf::Vector2f(headNode.getPosition().x + direction_.x * i * nodeRadius_ * 1.1,
											 headNode.getPosition().y + direction_.y * i * nodeRadius_ * 1.1)));
		/*nodes_.push_front(SnakeNode(
			headNode.getPosition().x + direction_.x * i * nodeRadius_ / 5.0,
			headNode.getPosition().y + direction_.y * i * nodeRadius_ / 5.0));*/
		if (tailOverlap_)
			tailOverlap_--;
		else
			nodes_.pop_back();
	}
	// debug
	debug = 0;
	for (auto node : nodes_)
	{
		std::cout << "node " << debug << " x: " << node.getPosition().x << " y: " << node.getPosition().y << std::endl;
		debug++;
	}
	// debug
}

void Snake::render(sf::RenderWindow &window)
{
	for (auto node : nodes_)
		node.render(window);
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