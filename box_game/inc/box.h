#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "box_map.h"
#include "tile_map.hpp"

class Box
{
    public:
        Box();
        void run();

    private:
        void processEvent();
	std::vector<sf::RectangleShape> update(sf::Time delatTime);
	void render(std::vector<sf::RectangleShape>& testVector);
	
    private:
        sf::RenderWindow mWindow;
        sf::View mView;
        sf::RectangleShape mPlayer;
        std::vector<sf::RectangleShape> mPlatforms;
	void ViewSmoothTransitionToPlayer();

        bool zoom;
        bool movingLeft;
        bool movingRight;
        bool jump;
        bool ground;
        float velocityY;
        int m_jumpCounter = 2;
        sf::Vector2f m_oldPosition;

        BoxMap mMap;
	TileMap mMapLevel0;
	
};
