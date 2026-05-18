#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "box_map.h"

class Box
{
    public:
        Box();
        void run();

    private:
        void processEvent();
        void update(sf::Time delatTime);
        void render();
    private:
        sf::RenderWindow mWindow;
        sf::View mView;
        sf::RectangleShape mPlayer;
        std::vector<sf::RectangleShape> mPlatforms;

        bool zoom;
        bool movingLeft;
        bool movingRight;
        bool jump;
        bool ground;
        float velocityY;
        int m_jumpCounter = 2;
        sf::Vector2f m_oldPosition;

        Map mMap;

};
