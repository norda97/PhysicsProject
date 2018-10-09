#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "Physics.h"

#define PI 3.14159265f

#define RADIUS 1.0f
const glm::vec2 virtSize(400.0f, 400.0f);

void constructArrow(Projectile& arrow)
{
	const float arrowHeadArea = 0.00034925f;
	const float mass = 0.018f;
	glm::vec3 startPos(RADIUS, 0.f, virtSize.y - 1.5f);
	arrow = { glm::vec3(0.0f), startPos, arrowHeadArea, mass, 0.04f };
}

void constructBow(Bow& bow)
{
	bow = { 1.25f, 0.07f, 470.f, 0.9f };
}

int main()
{
	Physics phys;
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	const float radius = 1.0f;
	sf::CircleShape shape(radius);
	shape.setFillColor(sf::Color::Green);

	float sx = shape.getRadius() / virtSize.x * (float)window.getSize().x;
	float sy = shape.getRadius() / virtSize.y * (float)window.getSize().y;
	shape.setRadius(sqrt(sx*sx+sy*sy));

	Bow bow;
	Projectile arrow;
	constructArrow(arrow);
	constructBow(bow);
	glm::vec3 dir(1.0f, 0.0f, -1.0f);
	float x = 1.0f;
	phys.applyBowForce(&arrow, &bow, dir, x);
	phys.addProjectile(&arrow);

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
		float x = arrow.pos.x / virtSize.x * (float)window.getSize().x;
		float y = arrow.pos.z / virtSize.y * (float)window.getSize().y;
		shape.setPosition(x, y);

		time += dt;
		if (time > 1.0f)
		{
			//printf("dt: %f, FPS: %f\n", dt, 1.f/dt);
			time = 0.0f;
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}