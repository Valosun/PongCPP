#include <SFML/Graphics.hpp>
#include <iostream>





int main()
{
	float SC_Height = 800;
	float SC_Width = 1280;
	std::cout << "Game Starting...";
	sf::RenderWindow window(sf::VideoMode(SC_Width, SC_Height), "Pong Game!");

#pragma region ScoreText
	int playerScore = 0;
	sf::Font font;
	if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
		std::cout << "Error loading font!" << std::endl;
		return -1;
	}
	sf::Text score;
	score.setFont(font);
	score.setString("SCORE: " + std::to_string(playerScore)); 
	score.setCharacterSize(40);
	score.setFillColor(sf::Color::White);
	score.setPosition(SC_Width / 2 - score.getGlobalBounds().width / 2, 20);
#pragma endregion

#pragma region Shapes&Sprites

  #pragma region ball
	sf::Vector2f ballPos(SC_Width / 2, SC_Height / 2);
	sf::Vector2f mainBallSpeed(8, 8);
	sf::Vector2f ballSpeed(mainBallSpeed);
	float ballSize = 15.f;
	sf::CircleShape ball(ballSize);
	ball.setOrigin(ballSize, ballSize);
	window.setVerticalSyncEnabled(true);
	ball.setFillColor(sf::Color::White);
	ball.setPosition(ballPos);
  #pragma endregion
  #pragma region paddles
	float paddleSizeX = 25;
	float paddleSizeY = 130;
	#pragma region Paddle1
	sf::Vector2f paddle1Pos(10, SC_Height / 2);
	int paddleSpeed = 8;
	sf::RectangleShape paddle1(sf::Vector2f(paddleSizeX, paddleSizeY));
	paddle1.setPosition(paddle1Pos);
	paddle1.setFillColor(sf::Color::White);
	paddle1.setOrigin(paddleSizeX / 2, paddleSizeY / 2);
	#pragma endregion

	#pragma region Paddle2
	sf::Vector2f paddle2Pos(SC_Width - 10, SC_Height / 2);
	sf::RectangleShape paddle2(sf::Vector2f(paddleSizeX, paddleSizeY));
	paddle2.setPosition(paddle2Pos);
	paddle2.setFillColor(sf::Color::White);
	paddle2.setOrigin(paddleSizeX / 2, paddleSizeY / 2);
	#pragma endregion

  #pragma endregion
  #pragma region midLine
	sf::Vertex midLine[] = {
	sf::Vertex(sf::Vector2f(SC_Width / 2, 0), sf::Color::White),
	sf::Vertex(sf::Vector2f(SC_Width / 2, SC_Height), sf::Color::White)
	};
  #pragma endregion

#pragma endregion


	window.setFramerateLimit(60);
	sf::Clock clock;

	while (window.isOpen())
	{
#pragma region Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			
		

#pragma region ScreenEvents
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				// Adjust the viewport when the window is resized
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
#pragma endregion

		}
#pragma endregion

#pragma region deltaTime
		//calculate the delta time
		sf::Time deltaTime = clock.restart();
		float deltaTimeSeconds = deltaTime.asSeconds();

		//make sure delta time stays within normal bounds, like between one FPS and zero FPS
		deltaTimeSeconds = std::min(deltaTimeSeconds, 1.f);
		deltaTimeSeconds = std::max(deltaTimeSeconds, 0.f);
#pragma endregion
	
#pragma region BallMovementUpdates
		ballPos.x += ballSpeed.x;
		ballPos.y += ballSpeed.y;
		if (ballPos.y - ballSize <= 0 || ballPos.y + ballSize >= SC_Height)
		{
			ballSpeed.y = -ballSpeed.y; 
		}
		if (ballPos.x - ballSize <= 0 || ballPos.x + ballSize >= SC_Width)
		{
			ballPos.x = SC_Width / 2;
			ballPos.y = SC_Height / 2;
			if (playerScore >= 1)
			{
				ballSpeed.x = ballSpeed.x / 2;
				ballSpeed.y = ballSpeed.y / 2;
			}
			playerScore--;
			if (playerScore < 0)
			{
				playerScore = 0;
			}
			score.setString("SCORE: " + std::to_string(playerScore));
			score.setPosition(SC_Width / 2 - score.getGlobalBounds().width / 2, 20);
		}
		ball.setPosition(ballPos);
#pragma endregion

#pragma region PaddleMovementUpdates
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)|| sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			paddle1Pos.y = paddle1Pos.y - paddleSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			paddle1Pos.y = paddle1Pos.y + paddleSpeed;
		}
		if (paddle1Pos.y <= 65)
		{
			paddle1Pos.y = 65;
		}
		if (paddle1Pos.y >= SC_Height - 65)
		{
			paddle1Pos.y = SC_Height - 65;
		}

		paddle1.setPosition(paddle1Pos);

		if (ballPos.y < paddle2Pos.y)
		{
			paddle2Pos.y -= (paddleSpeed - 0.3); 
		}
		else if (ballPos.y > paddle2Pos.y)
		{
			paddle2Pos.y += (paddleSpeed - 0.3); 
		}

		
		if (paddle2Pos.y <= 65)
		{
			paddle2Pos.y = 65;
		}
		if (paddle2Pos.y >= SC_Height - 65)
		{
			paddle2Pos.y = SC_Height - 65;
		}

		paddle2.setPosition(paddle2Pos);
#pragma endregion

#pragma region CollisionCheck
		if (paddle1.getGlobalBounds().intersects(ball.getGlobalBounds()))
		{
			ballSpeed.x = -mainBallSpeed.x;
			ballSpeed.x = -ballSpeed.x;
			playerScore++;
			if (playerScore % 3 == 0 && playerScore <= 12)
			{
				mainBallSpeed.x = mainBallSpeed.x + 3;
				mainBallSpeed.y = mainBallSpeed.y + 3;
			}
			
		
			
			score.setString("SCORE: " + std::to_string(playerScore)); 
			score.setPosition(SC_Width / 2 - score.getGlobalBounds().width / 2, 20); 
		}
		if (paddle2.getGlobalBounds().intersects(ball.getGlobalBounds()))
		{
			ballSpeed.x = mainBallSpeed.x;
			ballSpeed.x = -ballSpeed.x;
		}
#pragma endregion




#pragma region DrawArea
		window.clear();
		window.draw(midLine, 2, sf::Lines);
		window.draw(ball);
		window.draw(paddle1);
		window.draw(paddle2);
		window.draw(score);

		window.display();
#pragma endregion
	}



	return 0;
}