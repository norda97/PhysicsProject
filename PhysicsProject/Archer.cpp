#include "Archer.h"

#include "SFML/Window.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\norm.hpp"

#include "Utils.h"

Archer::Archer(const glm::vec3& pos) : sphere(0.25f)
{
	// Construct bow
	this->bow.mass = 1.25f;
	this->bow.c = 0.07f;
	this->bow.F = 407.f;
	this->bow.e = 0.9f;

	// Set direction
	this->dir = glm::vec3(1.f, 1.f, 0.0f);
	this->dir = glm::normalize(this->dir);

	this->strength = this->bow.F * 2;

	if (glm::length2(pos) < EPSILON)
		this->pos = glm::vec3(0.0f, 1.5f, 0.0f);
	else
		this->pos = pos;

	this->x = 0.0f;
	this->activated = true;
}

Archer::~Archer()
{
	unsigned int size = this->arrows.size();
	for (unsigned int i = 0; i < size; i++)
		delete this->arrows[i];
}

Projectile* Archer::addArrow()
{
	const float arrowHeadArea = 0.00034925f;
	const float mass = 4.f;//0.018f;
	
	// Construct arrow
	Projectile* arrow = new Projectile();
	arrow->vel = glm::vec3(0.0f);
	arrow->pos = this->pos;
	arrow->angVel = glm::vec3(0.0f);
	arrow->area = arrowHeadArea;
	arrow->mass = mass;
	arrow->cd = 0.04f;
	arrow->geometry = &sphere;
	
	this->arrows.push_back(arrow);
	return arrow;
}

void Archer::drawArrow(float x)
{
	this->x += x;
}

void Archer::shootArrow(Physics & physics)
{
	Projectile* arrow = addArrow();
	physics.applyBowForce(arrow, &this->bow, this->dir, this->x);
	physics.addProjectile(arrow);
}

void Archer::processInput(sf::RenderWindow& window, Physics& physics, float dt)
{
	sf::Vector2i mPos = sf::Mouse::getPosition(window);

	glm::vec2 pos = toScreenSpace(this->pos, window);

	glm::vec2 mv((float)mPos.x - pos.x, (float)mPos.y - pos.y);
	mv = glm::normalize(mv);
	this->dir.x = mv.x;
	this->dir.y = -mv.y;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		this->activated = false;
		drawArrow(0.1f);
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !this->activated)
	{
		this->activated = true;
		shootArrow(physics);
		this->x = 0;
	}
}

glm::vec3 Archer::getPos() const
{
	return this->pos;
}

glm::vec3 Archer::getDir() const
{
	return this->dir;
}

float Archer::getX() const
{
	return this->x;
}

std::vector<Projectile*>& Archer::getArrows()
{
	return this->arrows;
}
