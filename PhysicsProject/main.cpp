#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "Physics.h"
#include "Graphics.h"

#define TIME_STEP_FACTOR 0.25f 
#define PI 3.14159265f

#define RADIUS 1.0f
const glm::vec2 virtSize(10.0f, 10.0f);

void constructArrow(Projectile& arrow)
{
	/*
	const float arrowHeadArea = 0.00034925f;
	const float mass = 0.018f;
	glm::vec3 startPos(RADIUS, 1.5f, 0.0f); 
	arrow = { glm::vec3(0.0f), startPos, {0.0f, 0.0f, 0.0f}, arrowHeadArea, mass, 0.04f, Sphere(0.25f)};*/
}

void constructBow(Bow& bow)
{
	bow = { 1.25f, 0.07f, 470.f, 0.9f };
}

glm::vec2 toScreenSpace(const glm::vec3& v, sf::RenderWindow& window)
{
	float x = v.x / virtSize.x * (float)window.getSize().x;
	float y = (virtSize.y - v.y) / virtSize.y * (float)window.getSize().y;
	return glm::vec2(x, y);
}

glm::vec2 toScreenSpace(const glm::vec2& v, sf::RenderWindow& window)
{
	float x = v.x / virtSize.x * (float)window.getSize().x;
	float y = v.y / virtSize.y * (float)window.getSize().y;
	return glm::vec2(x, y);
}

float toScreenSpace(float v, sf::RenderWindow& window, unsigned int sideIndex = 0)
{
	glm::vec2 w(window.getSize().x, window.getSize().y);
	return v / virtSize[sideIndex] * (float)(w[sideIndex]);
}

int main()
{
	Physics phys;
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	/*
	Bow bow;
	Projectile arrow;
	constructArrow(arrow);
	constructBow(bow);
	glm::vec3 dir(1.0f, 0.0f, -1.0f);
	float x = 1.0f;
	phys.applyBowForce(&arrow, &bow, dir, x);
	phys.addProjectile(&arrow);
	*/
	float radius = 0.25f;
	Sphere sphere(0.25f);
	Cuboid cuboid({ 0.1f, 0.3f, 0.1f });

	Projectile ball1 = { {0.0f, -5.0f, 0.0f},{virtSize.x/2.0f + 0.15f, virtSize.y - radius, 0.0f}, {0.0f, 0.0f, 0.0f}, radius*radius*PI, 1.f, 0.4f, &sphere};
	Projectile ball2 = { {0.0f, 30.0f, 0.0f},{virtSize.x / 2.0f, radius, 0.0f}, {0.0f, 0.0f, 0.0f}, radius * radius * PI, 1.f, 0.4f, &cuboid};
	phys.addProjectile(&ball1);
	phys.addProjectile(&ball2);
	
	glm::vec2 r = toScreenSpace(glm::vec2(radius, radius), window);
	float radiusW = r.x;
	Graphics graphics(&window);

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
		phys.update(dt*TIME_STEP_FACTOR);
		t.restart();


		time += dt;
		if (time > 1.0f)
		{
			//printf("dt: %f, FPS: %f\n", dt, 1.f/dt);
			time = 0.0f;
		}

		window.clear(); 
		glm::vec2 ball1Pos = toScreenSpace(ball1.pos, window);
		glm::vec2 ball2Pos = toScreenSpace(ball2.pos, window); 
		graphics.drawBall(ball1Pos, toScreenSpace(((Sphere*)ball1.geometry)->radius, window, 0), sf::Color::Blue);
		graphics.drawRect(ball2Pos, toScreenSpace(((Cuboid*)ball2.geometry)->dim[0], window, 0)*2.f, 
			toScreenSpace(((Cuboid*)ball2.geometry)->dim[1], window, 1)*2.f, sf::Color::Red);
		//graphics.drawBall(ball2Pos, radiusW, sf::Color::Red);

		glm::vec2 dir2(ball2.vel.x, -ball2.vel.y);
		graphics.drawDbArrow(ball2Pos, dir2, 5.0f, 40.f, sf::Color::White);
		
		glm::vec2 dir1(ball1.vel.x, -ball1.vel.y);
		graphics.drawDbArrow(ball1Pos, dir1, 5.0f, 40.f, sf::Color::White);
		window.display();
	}

	return 0;
}