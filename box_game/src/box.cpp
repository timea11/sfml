#include "box.h"
#include <iostream>
#include <cmath>

std::array level0 = {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 3, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

Box::Box() :
mWindow(sf::VideoMode({512,256}),"Box Game")
,movingLeft(false)
,movingRight(false)
,jump(false)
,ground(true)
,velocityY(0.f)
,zoom(false)

{
    mView.setSize({200,200});
    mView.setCenter(mPlayer.getPosition());

    mPlayer.setSize({32,32});
    mPlayer.setOrigin({mPlayer.getSize().x /2, mPlayer.getSize().y /2}); //set center point of the player
    mPlayer.setPosition({50, 0});
    mPlayer.setFillColor(sf::Color::Magenta);

    sf::RectangleShape platform1;
    platform1.setSize({30,5});
    platform1.setPosition({110, 130});
    platform1.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform1);                                       //push_back - member function for std::vector
                                                                           //appends the given element to the end of the container
    sf::RectangleShape platform2;
    platform2.setSize({25,5});
    platform2.setPosition({30, 90});
    platform2.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform2);

    sf::RectangleShape platform3;
    platform3.setSize({20,5});
    platform3.setPosition({75, 60});
    platform3.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform3);

    sf::RectangleShape platform4;
    platform4.setSize({20,5});
    platform4.setPosition({400, 80});
    platform4.setFillColor(sf::Color::Cyan);
    mPlatforms.push_back(platform4);

    // create the tilemap from the level definition
    if (!mMapLevel0.load("media/tileset.png", {32, 32}, level0.data(), 16, 8))
	    std::cout << "Error: there is an error loading level0 map." << std::endl;
    
}
void Box::run()
{
    sf::Clock clock;

    while(mWindow.isOpen())         //infinite loop to be open
    {
        sf::Time deltaTime = clock.restart();

        processEvent();
	std::vector<sf::RectangleShape> testVectors = update(deltaTime);
        render(testVectors);
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


std::vector<sf::RectangleShape> Box::update(sf::Time deltaTime)
{
    const float PlayerSpeed = 100.f;
    const float jumpHeight = -200.f;
    const float gravity = 400.f;
    const float groundY = 240.f;

    std::vector<sf::RectangleShape> testVector;
    

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

    int* mapLvl0_matrix = mMapLevel0.getTilesMatrix();
    std::vector<int> mapLvl0_dim = mMapLevel0.getTilesMatrixDim();

    int tilesMatrixCX = std::floor(mPlayer.getPosition().x / 32.0);
    int tilesMatrixCY = std::floor(mPlayer.getPosition().y / 32.0);
    int tilesMatricIndex = tilesMatrixCX + tilesMatrixCY * mapLvl0_dim[0];
    int tileType = mapLvl0_matrix[tilesMatricIndex];

    int tileTypeAbove;

    sf::VertexArray mapVertices = mMapLevel0.getMapVertices();

    int shifterArray[] = {-1, 0, 1};
    for(int c = 0; c <= 2; c++)
    {
	for(int t = 0; t <= 2; t++)
	{
		int xShift = tilesMatrixCX + shifterArray[c];
		int yShift = tilesMatrixCY + shifterArray[t];
		tilesMatricIndex = (xShift) + (yShift) * mapLvl0_dim[0];
		tileType = mapLvl0_matrix[tilesMatricIndex];

		int yShiftAbove = (yShift - 1) < 0 ? 0 : yShift - 1;
		int tilesMatricIndexAbove = (xShift) + (yShiftAbove) * mapLvl0_dim[0];
		tileTypeAbove = mapLvl0_matrix[tilesMatricIndexAbove];

		if (tileType == 3)
		{
			sf::Vertex* triangles = &mapVertices[tilesMatricIndex * 6];
			sf::Vector2f p1 = triangles[0].position;
			sf::Vector2f p2 = triangles[1].position;

			// Create a mini bounding box for this specific line segment
			float left = std::min(p1.x, p2.x);
			float top = std::min(p1.y, p2.y);

			sf::RectangleShape lineBoundLeftSide({3, 32});
			lineBoundLeftSide.setPosition({left, top});
			lineBoundLeftSide.setFillColor(sf::Color::Blue);
			testVector.push_back(lineBoundLeftSide);

			if (tilesMatrixCY < yShift)
			{
			    sf::RectangleShape lineBoundTop({32, 3});
			    lineBoundTop.setPosition({left, top});
			    lineBoundTop.setFillColor(sf::Color::Red);
			    testVector.push_back(lineBoundTop);

			    if(velocityY > 0.f)                                                 //box in falling stage
			    {
				if (mPlayer.getGlobalBounds().findIntersection(lineBoundTop.getGlobalBounds())) {
				    mPlayer.setPosition(                                            //set position to the current player state for x, and position of the plraform by y
					{mPlayer.getPosition().x,
					 lineBoundTop.getGlobalBounds().position.y - mPlayer.getSize().y / 2}
				    );

				    velocityY = 0.f;                                                //velocity back to 0, because when it stop on platform, no velocity
				    ground = true;
				    m_jumpCounter = 2;
				    return testVector;
				}
			    }

			    if (mPlayer.getGlobalBounds().findIntersection(lineBoundLeftSide.getGlobalBounds()) && movingRight && velocityY <= 0) {

					mPlayer.setPosition(                                            
					    {mPlayer.getPosition().x - 3,
						mPlayer.getPosition().y}        
					);

				    return testVector;
			    }
				
			} else {
			    if (mPlayer.getGlobalBounds().findIntersection(lineBoundLeftSide.getGlobalBounds()) && movingRight) {

					mPlayer.setPosition(                                            //set position to the current player state for x, and position of the plraform by y
					    {mPlayer.getPosition().x - 3,
						mPlayer.getPosition().y}        //but platform y is bottom line of the platform, thats why we have to substrac y 2.f
					);

				    return testVector;
			    }

			    if (yShift != 0 && tileTypeAbove != 3)
			    {
				    sf::RectangleShape lineBoundTop({32, 3});
				    lineBoundTop.setPosition({left, top});
				    lineBoundTop.setFillColor(sf::Color::Red);
				    testVector.push_back(lineBoundTop);

				    if (mPlayer.getGlobalBounds().findIntersection(lineBoundTop.getGlobalBounds())) {
					    if(velocityY > 0.f)                                                 //box in falling stage
					    {
                                              mPlayer.setPosition(
                                                  {mPlayer.getPosition().x,
                                                   lineBoundTop.getGlobalBounds().position.y - mPlayer.getSize().y / 2}
                                                  );

						velocityY = 0.f;
						ground = true;
						m_jumpCounter = 2;
					    }
				    }
			    }
				
			}

		}

	}
	    
    }

    return testVector;
}
void Box::render(std::vector<sf::RectangleShape>& testVector)
{
    mWindow.clear();

    mWindow.setView(mView);

    mWindow.draw(mMapLevel0);

    mWindow.draw(mPlayer);
    for(auto v: testVector)
    {
	    mWindow.draw(v);
    }
    mWindow.display();
}
