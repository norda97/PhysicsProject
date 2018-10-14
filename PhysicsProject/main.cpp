#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "Physics.h"
#include "Graphics.h"
#include "Archer.h"

#define TIME_STEP_FACTOR 1.0f 
#define PI 3.14159265f

#include "Utils.h"

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

	Projectile ball1 = { {0.0f, -5.0f, 0.0f},{virtSize.x/2.0f, virtSize.y - radius, 0.0f}, {0.0f, 0.0f, 0.0f}, radius*radius*PI, 1.f, 0.4f, &sphere};
	Projectile ball2 = { {0.0f, 0.0f, 0.0f},{virtSize.x / 2.0f, virtSize.y - radius*4.f, 0.0f}, {0.0f, 0.0f, 0.0f}, radius * radius * PI, 1.f, 0.4f, &sphere};
	phys.addProjectile(&ball1);
	phys.addProjectile(&ball2);
	
	Archer archer;
	Archer archer2({virtSize.x, 1.5f, 0.0f});

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
		static float angle = 0.0f;
		angle += ball1.angVel.z*dt*TIME_STEP_FACTOR;
		graphics.drawBall(ball1Pos, toScreenSpace(((Sphere*)ball1.geometry)->radius, window, 0), sf::Color::Blue, angle);
		//graphics.drawRect(ball2Pos, toScreenSpace(((Cuboid*)ball2.geometry)->dim[0], window, 0)*2.f, 
		//	toScreenSpace(((Cuboid*)ball2.geometry)->dim[1], window, 1)*2.f, sf::Color::Red);
		static float angle2 = 0.0f;
		angle2 += ball2.angVel.z*dt*TIME_STEP_FACTOR;
		graphics.drawBall(ball2Pos, toScreenSpace(((Sphere*)ball2.geometry)->radius, window, 0), sf::Color::Red, angle2);

		glm::vec2 dir2(ball2.vel.x, -ball2.vel.y);
		graphics.drawDbArrow(ball2Pos, dir2, 5.0f, 40.f, sf::Color::White);
		
		glm::vec2 dir1(ball1.vel.x, -ball1.vel.y);
		graphics.drawDbArrow(ball1Pos, dir1, 5.0f, 40.f, sf::Color::White);
		
		archer.processInput(window, phys, dt);
		archer2.processInput(window, phys, dt);

		std::vector<Projectile*>& arrows = archer.getArrows();
		for (Projectile* arrow : arrows)
			graphics.drawBall(toScreenSpace(arrow->pos, window), 
				toScreenSpace(((Sphere*)arrow->geometry)->radius, window, 0), sf::Color::Cyan);

		std::vector<Projectile*>& arrows2 = archer2.getArrows();
		for (Projectile* arrow : arrows2)
			graphics.drawBall(toScreenSpace(arrow->pos, window),
				toScreenSpace(((Sphere*)arrow->geometry)->radius, window, 0), sf::Color::Cyan);

		graphics.drawDbArrow(toScreenSpace(archer.getPos(), window), 
			glm::vec2(archer.getDir().x, -archer.getDir().y), 10.f + archer.getX(), 40.f);
		graphics.drawDbArrow(toScreenSpace(archer2.getPos(), window),
			glm::vec2(archer2.getDir().x, -archer2.getDir().y), 10.f + archer2.getX(), 40.f);
		
		window.display();
	}

	return 0;
}