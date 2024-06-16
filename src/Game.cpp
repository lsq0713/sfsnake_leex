#include <SFML/Graphics.hpp>

#include <memory>

#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 10.f);

std::shared_ptr<Screen> Game::GameScreen = std::make_shared<MenuScreen>();

sf::Clock Game::mouseButtonClock = sf::Clock();
sf::Time Game::mouseCD = sf::Time();
bool Game::mouseButtonLocked = false;

sf::Clock Game::keyButtonClock = sf::Clock();
sf::Time Game::keyCD = sf::Time();
bool Game::keyButtonLocked = false;
bool Game::GridVisibility = false;
int Game::GridColor = 1;
int Game::BgColor = 0;

Game::Game()
	: window_(GameVideoMode, "sfSnake")
{
	bgMusic_.openFromFile("Music/bg_music.wav");
	bgMusic_.setLoop(true);
	bgMusic_.play();
	
}

/* static Golbal VideoMode settings
 * 静态全局视频设置，便于创建合适大小的窗口
 * 并且在界面初始化时定位组件
 */

sf::VideoMode Game::initVideoMode_()
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	// printf("desktop width: %d, height: %d\n", desktopMode.width, desktopMode.height);
	return sf::VideoMode(
		desktopMode.width / 2.0f,
		desktopMode.height / 2.0f,
		desktopMode.bitsPerPixel);
}

sf::VideoMode Game::GameVideoMode = Game::initVideoMode_();


int Game::Width = sf::VideoMode::getDesktopMode().width / 2.0f;
int Game::Height = sf::VideoMode::getDesktopMode().height / 2.0f;
sf::Font Game::GameFont = sf::Font();

void Game::handleInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window_.close();
	}

	Game::GameScreen->handleInput(window_);
}

void Game::update(sf::Time delta)
{
	Game::GameScreen->update(delta);
}

void Game::render()
{
	window_.clear(BgColors[BgColor]);
	Game::GameScreen->render(window_);
	window_.display();
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window_.isOpen())
	{
		sf::Time delta = clock.restart();
		timeSinceLastUpdate += delta;
		mouseButtonClock.restart();
		keyButtonClock.restart();
		while (timeSinceLastUpdate > Game::TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			handleInput();
			update(TimePerFrame);
			render();
		}
		delta = mouseButtonClock.restart();
		Game::mouseCD += delta;
		if (Game::mouseCD.asSeconds() > 0.3f)
		{
			mouseCD -= sf::seconds(0.3f);
			mouseButtonLocked = false;
		}
		delta = keyButtonClock.restart();
		Game::keyCD += delta;
		if (Game::keyCD.asSeconds() > 0.3f)
		{
			keyCD -= sf::seconds(0.3f);
			keyButtonLocked = false;
		}
	}
}