#include <SFML/Graphics.hpp>

#include <memory>
#include <typeinfo>// 调试用

#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 10.f);

std::shared_ptr<Screen> Game::GameScreen = std::make_shared<MenuScreen>();

bool Game::mouseButtonLocked = false;

Game::Game()
: window_(sf::VideoMode(Game::Width, Game::Height), "sfSnake")
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
    return sf::VideoMode(
        desktopMode.width / 2.0f,
        desktopMode.height / 2.0f,
        desktopMode.bitsPerPixel);
}

sf::VideoMode Game::GameVideoMode = Game::initVideoMode_();

void Game::handleInput()
{
	sf::Event event;
	printf("Game begin handleInput\n");
	while (window_.pollEvent(event))
	{
		printf("Game inside handleInput\n");
		if (event.type == sf::Event::Closed)
			window_.close();
	}
	Game::GameScreen->handleInput(window_);
	printf("%s\n", typeid(*Game::GameScreen).name());
	printf("Game outside handleInput\n");
}

void Game::update(sf::Time delta)
{
	Game::GameScreen->update(delta);
}

void Game::render()
{
	window_.clear();
	Game::GameScreen->render(window_);
	window_.display();
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	printf("Game before window_.isOpen()\n");
	while (window_.isOpen())
	{
		sf::Time delta = clock.restart();
		timeSinceLastUpdate += delta;
		// printf("Game before timeSinceLastUpdate > Game::TimePerFrame\n");
		while (timeSinceLastUpdate > Game::TimePerFrame)
		{
			printf("Game inside run\n");
			timeSinceLastUpdate -= TimePerFrame;
			handleInput();
			update(TimePerFrame);
		}

		render();
	}
}