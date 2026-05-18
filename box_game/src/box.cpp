#include "box.h"
#include "box_map.h"

Box::Box() :
mWindow(sf::VideoMode({200,200}),"EG800Q")
,movingLeft(false)
,movingRight(false)
,jump(false)
,ground(true)
,velocityY(0.f)
,zoom(false)

{
    mView.setSize({200,200});
    mView.setCenter(mPlayer.getPosition());

    mPlayer.setSize(sf::Vector2f(10,10));
    mPlayer.setOrigin({mPlayer.getSize().x /2, mPlayer.getSize().y /2}); //set center point of the player
    mPlayer.setPosition({50, 150});
    mPlayer.setFillColor(sf::Color::Magenta);

    sf::RectangleShape platform1;
    platform1.setSize(sf::Vector2f(30,5));
    platform1.setPosition({110, 130});
    platform1.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform1);                                       //push_back - member function for std::vector
                                                                           //appends the given element to the end of the container
    sf::RectangleShape platform2;
    platform2.setSize(sf::Vector2f(25,5));
    platform2.setPosition({30, 90});
    platform2.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform2);

    sf::RectangleShape platform3;
    platform3.setSize(sf::Vector2f(20,5));
    platform3.setPosition({75, 60});
    platform3.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform3);

    sf::RectangleShape platform4;
    platform4.setSize(sf::Vector2f(20,5));
    platform4.setPosition({400, 80});
    platform4.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform4);
}
void Box::run()
{
    sf::Clock clock;

    while(mWindow.isOpen())         //infinite loop to be open
    {
        sf::Time deltaTime = clock.restart();

        processEvent();
        update(deltaTime);
        render();
    }
}

void Box::processEvent()
{
    // pollEvent now returns std::optional<sf::Event>, loop continues while it has a value
    while (const std::optional<sf::Event> event = mWindow.pollEvent())
    {
        // 1. Check for Window Closed
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close();
        }

        // 2. Check for Key Pressed and extract key data
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::A)       // Scoped enum: Key::A
            {
                movingLeft = true;
            }
            if (keyPressed->code == sf::Keyboard::Key::D)       // Scoped enum: Key::D
            {
                movingRight = true;
            }
            if (keyPressed->code == sf::Keyboard::Key::W)
            {
                jump = true;
            }
            if (keyPressed->code == sf::Keyboard::Key::Z)
            {
                zoom = true;
            }
        }

        // 3. Check for Key Released and extract key data
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->code == sf::Keyboard::Key::A)
            {
                movingLeft = false;
            }
            if (keyReleased->code == sf::Keyboard::Key::D)
            {
                movingRight = false;
            }
            if (keyReleased->code == sf::Keyboard::Key::W)
            {
                jump = false;
            }
        }
    }
}


void Box::update(sf::Time deltaTime)
{
    const float PlayerSpeed = 100.f;
    const float jumpHeight = -200.f;
    const float gravity = 400.f;
    const float groundY = 175.f;

    mView.setCenter(mPlayer.getPosition());
    sf::Vector2f movement(0.f,0.f);

    if(zoom)
    {
        mView.zoom(0.7f);
        zoom = false;
    }

    if(movingLeft)
        movement.x -= PlayerSpeed;

    if(movingRight)
        movement.x += PlayerSpeed;
    
    if(jump && ground)
    {
        jump = false;
        ground = false;

        velocityY = jumpHeight;
        m_oldPosition = mPlayer.getPosition();
        m_jumpCounter--;
    }
    else if (jump && m_jumpCounter == 1 && mPlayer.getPosition().y < (m_oldPosition.y)-20)
    {
        jump = false;
        velocityY  = -200;
        if (m_jumpCounter >= 1)
        {
            m_jumpCounter--;
        }
    }
    

    velocityY += gravity * deltaTime.asSeconds();                  //apply gravity
    movement.y = velocityY;                                        //apply vertical velocity
    mPlayer.move(movement * deltaTime.asSeconds());

    // update values when player hits the floor
    if(mPlayer.getPosition().y >= groundY)
    {
       mPlayer.setPosition({mPlayer.getPosition().x, groundY});
       ground = true;
       m_jumpCounter = 2;
       velocityY = 0.f;
    }

    // update values when player hit and top of the tile
    for(auto& tile : mMap.getTiles())                                            //for every item in vector mPlatform, give me reference called platform
    {                                                                           //intersect - presecati
        if(mPlayer.getGlobalBounds().findIntersection(tile.getGlobalBounds()))    //if bounds of player and of the platform are intersects each other
        {
            if(velocityY > 0.f)                                                 //box in falling stage
            {
                mPlayer.setPosition(                                            //set position to the current player state for x, and position of the plraform by y
                    {mPlayer.getPosition().x,
                    tile.getPosition().y - mPlayer.getSize().y / 2}        //but platform y is bottom line of the platform, thats why we have to substrac y 2.f
                );

                velocityY = 0.f;                                                //velocity back to 0, because when it stop on platform, no velocity
                ground = true;
		m_jumpCounter = 2;
            }
        }
    }


}
void Box::render()
{
    mWindow.clear(sf::Color::Black);
    mWindow.setView(mView);
    mWindow.draw(mPlayer);
    // for(auto& platform : mPlatforms)
    // {
    //     mWindow.draw(platform);
    // }
    for(auto& tile : mMap.getTiles())
    {
        mWindow.draw(tile);
    }
    mMap.draw(mWindow);
    mWindow.display();

}
