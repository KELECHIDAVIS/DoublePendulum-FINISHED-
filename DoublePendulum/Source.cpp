#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>; 
#include <iostream>
#include <cmath> 

struct Coords
{
	int x, y;
	Coords()
	{}
	Coords(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

void drawLine(sf::RenderWindow& window, std::vector<sf::Vertex> vertices)
{
	for (int i = 1; i < vertices.size(); i++)
	{
		sf::Vertex temp[2] = {
			vertices[i - 1],vertices[i]
		};
		window.draw(temp, 2, sf::Lines);
	}
}

int main()
{
	int width = 1250, height = 600, originCircleSize = 4, length1 = 250, length2 = 250, mass1 = 20, mass2 = 20, frameLimit = 60;  // theta being the angle between the rod and the vertical;
	double theta1 = M_PI / 2, theta2 = M_PI/2,
		theta1v = 0, theta2v = 0, g = 1,
		theta1a, theta2a;
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(width, height), "Double Pendulum Simulation");
	sf::Event event;
	sf::Color lineColor = sf::Color::Red, traceColor = sf::Color::Black;
	sf::CircleShape originCircle(originCircleSize);
	originCircle.setFillColor(sf::Color::Black);
	originCircle.setOrigin(originCircleSize, originCircleSize);
	Coords coords1, coords2, origin(width / 2, height / 12); // the origin is going to be the point that the first pendulum is hanging from so it will need to be added to every reference of coordinates 
	originCircle.setPosition(sf::Vector2f(origin.x, origin.y));
	std::vector<sf::Vertex> traceVertices;
	//first pendulum 
	sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(origin.x,origin.y),lineColor),
		sf::Vertex(sf::Vector2f(origin.x + length1 * sin(theta1),origin.y + length1 * cos(theta1)),lineColor) };
	sf::CircleShape bob1(mass1);
	bob1.setOrigin(mass1, mass1);
	bob1.setFillColor(sf::Color::Black);
	bob1.setPosition(sf::Vector2f(origin.x + length1 * sin(theta1), origin.y + length1 * cos(theta1)));

	//second pendulum 
	sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(origin.x + cos(length1),origin.y + sin(length1)),lineColor),
		sf::Vertex(sf::Vector2f(origin.x + length1 * sin(theta1) + length2 * sin(theta2),origin.y + length1 * cos(theta1) + length2 * cos(theta2)) ,lineColor) };
	sf::CircleShape bob2(mass2);
	bob2.setOrigin(mass1, mass1);
	bob2.setFillColor(sf::Color::Black);
	bob2.setPosition(sf::Vector2f(origin.x + length1 * sin(theta1) + length2 * sin(theta2), origin.y + length1 * cos(theta1) + length2 * cos(theta2)));


	window.setFramerateLimit(frameLimit);
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;

			}
		}
		sf::Time time = clock.getElapsedTime();
		std::cout << 1.f / time.asSeconds() << std::endl;

		//update 

		double num1 = -g * (2 * mass1 + mass2) * sin(theta1);
		double num2 = -mass2 * g * sin(theta1 - 2 * theta2);
		double num3 = -2 * sin(theta1 - theta2) * mass2;
		double num4 = (theta2v * theta2v) * length2 + (theta1v * theta1v) * length1 * cos(theta1 - theta2);
		double firstNum = num1 + num2 + num3 * num4;
		double firstDen = length1 * (2 * mass1 + mass2 - mass2 * cos(2 * theta1 - 2 * theta2));
		theta1a = firstNum / firstDen;

		double n1 = 2 * sin(theta1 - theta2);
		double n2 = (theta1v * theta1v) * length1 * (mass1 + mass2);
		double n3 = g * (mass1 + mass2) * cos(theta1);
		double n4 = (theta2v * theta2v) * length2 * mass2 * cos(theta1 - theta2);
		double secondNum = n1 * (n2 + n3 + n4);
		double secondDen = length2 * (2 * mass1 + mass2 - mass2 * cos(2 * theta1 - 2 * theta2));
		theta2a = secondNum / secondDen;

		theta1v += theta1a;
		theta2v += theta2a;
		theta1 += theta1v;
		theta2 += theta2v;

		//change line orientations
		line1[1] = sf::Vertex(sf::Vector2f(origin.x + length1 * sin(theta1), origin.y + length1 * cos(theta1)), lineColor);
		line2[0] = sf::Vertex(sf::Vector2f(origin.x + length1 * sin(theta1), origin.y + length1 * cos(theta1)), lineColor);
		line2[1] = sf::Vertex(sf::Vector2f(origin.x + length1 * sin(theta1) + length2 * sin(theta2), origin.y + length1 * cos(theta1) + length2 * cos(theta2)), lineColor);

		//change bob position 
		bob1.setPosition(sf::Vector2f(origin.x + length1 * sin(theta1), origin.y + length1 * cos(theta1)));
		bob2.setPosition(sf::Vector2f(origin.x + length1 * sin(theta1) + length2 * sin(theta2), origin.y + length1 * cos(theta1) + length2 * cos(theta2)));

		// trace 
		traceVertices.push_back(sf::Vertex(sf::Vector2f(origin.x + length1 * sin(theta1) + length2 * sin(theta2), origin.y + length1 * cos(theta1) + length2 * cos(theta2)), traceColor));


		clock.restart().asSeconds();
		//draw 
		window.clear(sf::Color::White);
		window.draw(line1, 2, sf::Lines);
		window.draw(line2, 2, sf::Lines);
		window.draw(bob1);
		window.draw(bob2);
		drawLine(window, traceVertices);
		window.draw(originCircle);
		window.display();

	}

	return 0;
}