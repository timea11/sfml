#include "box.h"
#include "box_map.h"

Box::Box() :
mWindow(sf::VideoMode(200,200),"EG800Q")
,movingLeft(false)
,movingRight(false)
,jump(false)
,ground(true)
,velocityY(0.f)
,zoom(false)

{
    mView.setSize(200.f,200.f);
    mView.setCenter(mPlayer.getPosition());

    mPlayer.setSize(sf::Vector2f(10,10));
    mPlayer.setOrigin(mPlayer.getSize().x /2.f, mPlayer.getSize().y /2.f); //set center point of the player
    mPlayer.setPosition(50.f, 150.f);
    mPlayer.setFillColor(sf::Color::Magenta);

    sf::RectangleShape platform1;
    platform1.setSize(sf::Vector2f(30,5));
    platform1.setPosition(110.f, 130.f);
    platform1.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform1);                                       //push_back - member function for std::vector
                                                                           //appends the given element to the end of the container
    sf::RectangleShape platform2;
    platform2.setSize(sf::Vector2f(25,5));
    platform2.setPosition(30.f, 90.f);
    platform2.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform2);

    sf::RectangleShape platform3;
    platform3.setSize(sf::Vector2f(20,5));
    platform3.setPosition(75.f, 60.f);
    platform3.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform3);

    sf::RectangleShape platform4;
    platform4.setSize(sf::Vector2f(20,5));
    platform4.setPosition(400.f, 80.f);
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
    sf::Event event;                                    //event processing
    while(mWindow.pollEvent(event))                     //infinite loop to capture events poll - pocahnuc
    {
       if(event.type == sf::Event::Closed)
           mWindow.close();

       if(event.type == sf::Event::KeyPressed)
       {
           if(event.key.code == sf::Keyboard::A)        //if it is pressed keyboard arow left
           {
                movingLeft = true;                      //flag true for left
           }
           if(event.key.code == sf::Keyboard::D)        //if it is pressed keyboard arow right
           {
                movingRight = true;                     //flag true for right
           }
           if(event.key.code == sf::Keyboard::W && ground)
           {
                jump = true;
           }
           if(event.key.code == sf::Keyboard::Z)
           {
               zoom = true;
           }
       }
       if(event.type == sf::Event::KeyReleased)
       {
           if(event.key.code ==sf::Keyboard::A)
           {
               movingLeft = false;                      //flag false for left
           }
           if(event.key.code == sf::Keyboard::D)
           {
               movingRight = false;                     //flag false for right
           }
           if(event.key.code == sf::Keyboard::W)
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
    const float groundY = 150.f;

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
        velocityY = jumpHeight;
        ground = false;
    }

    velocityY += gravity * deltaTime.asSeconds();                  //apply gravity
    movement.y = velocityY;                                        //apply vertical velocity
    mPlayer.move(movement * deltaTime.asSeconds());

    if(mPlayer.getPosition().y >= groundY)
    {
       mPlayer.setPosition(mPlayer.getPosition().x, groundY);
       ground = true;
       velocityY = 0.f;
    }

    for(auto& tile : mMap.getTiles())                                            //for every item in vector mPlatform, give me reference called platform
    {                                                                           //intersect - presecati
        if(mPlayer.getGlobalBounds().intersects(tile.getGlobalBounds()))    //if bounds of player and of the platform are intersects each other
        {
            if(velocityY > 0.f)                                                 //box in falling stage
            {
                mPlayer.setPosition(                                            //set position to the current player state for x, and position of the plraform by y
                    mPlayer.getPosition().x,
                    tile.getPosition().y - mPlayer.getSize().y / 2.f        //but platform y is bottom line of the platform, thats why we have to substrac y 2.f
                );

                velocityY = 0.f;                                                //velocity back to 0, because when it stop on platform, no velocity
                ground = true;
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
