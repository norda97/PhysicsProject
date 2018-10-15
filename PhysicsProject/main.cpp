#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "Physics.h"
#include "Graphics.h"
#include "Archer.h"

#define TIME_STEP_FACTOR 0.1f 
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
	float radius = 0.4f;
	Sphere sphere(0.4f);
	Cuboid cuboid({ 0.3f, 0.1, 0.1f });

	Projectile ball1 = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},{virtSize.x/2.0f, virtSize.y - radius, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, radius*radius*PI, 5.0f, 0.4f, &sphere};
	Projectile ball2 = { {0.0f, 0.0f, 0.0f} ,{0.0f, 20.0f, 0.0f},{virtSize.x - radius, radius*4.f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2f*0.6*0.2f, 5.f, 2.05f, &cuboid };
	phys.addProjectile(&ball1);
	phys.addProjectile(&ball2);
	/*
	Cuboid floorCuboid({ virtSize.x / 2.0f, 0.25f, 0.25f });
	Projectile floor;
	floor.pos = { virtSize.x / 2.0f, -0.25f, 0.0f};
	floor.geometry = &floorCuboid;
	phys.addProjectile(&floor);
	*/
	Archer archer;

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

		// TODO: Angle.z should be negated if want correct direction, I don´t know how the other should be yet. Has to do with converting from virtual coordinate to screen coortinate, because of change in y.
		//printf("Ang: %f\n", -ball2.ang.z);
		graphics.drawBall(ball1Pos, toScreenSpace(((Sphere*)ball1.geometry)->radius, window, 0), sf::Color::Blue, -ball1.ang.z);
		graphics.drawRect(ball2Pos, toScreenSpace(((Cuboid*)ball2.geometry)->dim[0], window, 0)*2.f, 
			toScreenSpace(((Cuboid*)ball2.geometry)->dim[1], window, 1)*2.f, sf::Color::Red, -ball2.ang.z);
		//graphics.drawBall(ball2Pos, toScreenSpace(((Sphere*)ball2.geometry)->radius, window, 0), sf::Color::Red, angle2);

		glm::vec2 dir2(ball2.vel.x, -ball2.vel.y);
		graphics.drawDbArrow(ball2Pos, dir2, 5.0f, 40.f, sf::Color::White);
		
		glm::vec2 dir1(ball1.vel.x, -ball1.vel.y);
		graphics.drawDbArrow(ball1Pos, dir1, 5.0f, 40.f, sf::Color::White);
		
		archer.processInput(window, phys, dt);

		std::vector<Projectile*>& arrows = archer.getArrows();
		for (Projectile* arrow : arrows)
		{
			LineSegment* lineSegment = (LineSegment*)arrow->geometry;
			glm::vec2 dir(arrow->dir.x, -arrow->dir.y);
			if (arrow->hasPhysics)
			{
				graphics.drawLineSegment(toScreenSpace(arrow->pos, window),
					dir, toScreenSpace(lineSegment->d, window), sf::Color::Cyan);
			}
			else
			{
				// TODO: Add rotation on arrow->pos and dir.
				graphics.drawLineSegment(toScreenSpace(arrow->pos + arrow->parent->pos, window),
					dir, toScreenSpace(lineSegment->d, window), sf::Color::Cyan);
			}
		}
		graphics.drawDbArrow(toScreenSpace(archer.getPos(), window), 
			glm::vec2(archer.getDir().x, -archer.getDir().y), 10.f + archer.getX()*100.f, 200.f);
		
		window.display();
	}

	return 0;
}