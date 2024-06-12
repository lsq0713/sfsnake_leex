#include "Grid.h"
#include "Game.h"

using namespace sfSnake;


Grid::Grid()
    : gridVline(sf::Vector2f(Game::GameVideoMode.width / 500.0f, Game::GameVideoMode.height)),
      gridHline(sf::Vector2f(Game::GameVideoMode.width, Game::GameVideoMode.width / 500.0f))
{
}

void Grid::render(sf::RenderWindow &window)
{
    gridVline.setFillColor(GridColors[Game::GridColor]);
    gridHline.setFillColor(GridColors[Game::GridColor]);

    static float gridSpace = Game::GameVideoMode.width / 50.0f;
    for (float i = 0.f; i < Game::GameVideoMode.width; i += gridSpace)
    {
        gridVline.setPosition(i, 0);
        window.draw(gridVline);
    }

    for (float i = 0.f; i < Game::GameVideoMode.height; i += gridSpace)
    {
        gridHline.setPosition(0, i);
        window.draw(gridHline);
    }
}
