#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "Physics.h"

#define PI 3.14159265f


int main()
{
	Physics phys;
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	const float radius = 0.5f;
	sf::CircleShape shape(radius);
	shape.setFillColor(sf::Color::Green);

	glm::vec2 virtSize(50.f, 50.f);
	float sx = shape.getRadius() / virtSize.x * (float)window.getSize().x;
	float sy = shape.getRadius() / virtSize.y * (float)window.getSize().y;
	shape.setRadius(sqrt(sx*sx+sy*sy));
	
	Projectile ball = { glm::vec3(100.0f, 0.0f, -100.f), glm::vec3(radius, 0.f, virtSize.y-radius), radius*radius * PI, 1.f, 0.01f };
	phys.addProjectile(&ball);


	sf::Clock t;
	float time = 0.0f;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = t.getElapsedTime().asSeconds();
		phys.update(dt);
		t.restart();
		float x = ball.pos.x / virtSize.x * (float)window.getSize().x;
		float y = ball.pos.z / virtSize.y * (float)window.getSize().y;
		shape.setPosition(x, y);

		time += dt;
		if (time > 1.0f)
		{
			printf("dt: %f, FPS: %f\n", dt, 1.f/dt);
			time = 0.0f;
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}