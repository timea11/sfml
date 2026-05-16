#include "box_map.h"

Map::Map()
{
    int level[9][10] =
    {
        {0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,1},
        {1,0,0,1,1,1,1,0,0,1},
        {1,0,1,1,1,1,1,0,1,1},
        {1,1,1,1,1,1,1,1,1,1},
    };

    float tileSize = 20.f;

    for(int row = 0; row < 9; row++)
    {
        for(int col = 0; col < 10; col++)
        {
            if(level[row][col] == 1)
            {
                sf::RectangleShape tile;
                tile.setSize(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(
                    {col* tileSize,
                    row * tileSize}
                );

                tile.setFillColor(sf::Color::Green);
                mTiles.push_back(tile);
            }
        }
    }
}

void Map::draw(sf::RenderWindow& window)
{
    for(auto& tile : mTiles)
    {
        window.draw(tile);
    }
}

std::vector<sf::RectangleShape>& Map::getTiles()
{
    return mTiles;
}


