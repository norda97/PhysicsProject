#ifndef ARCHER_H
#define ARCHER_H

#include <vector>
#include "Projectile.h"
#include "Physics.h"
#include "SFML/Graphics.hpp"

class Archer
{
public:
	Archer(const glm::vec3& pos = glm::vec3(0.0f));
	~Archer();

	void shootArrow(Physics& physics);

	void processInput(sf::RenderWindow& window, Physics & physics, float dt);

	glm::vec3 getPos() const;
	glm::vec3 getDir() const;
	float getX() const;

	std::vector<Projectile*>& getArrows();

private:
	void drawArrow(float x);
	Projectile* addArrow();

	float x;
	float fullDraw;
	bool isFullDrawn;
	float energy;
	glm::vec3 dir;
	glm::vec3 pos;

	Bow bow;
	std::vector<Projectile*> arrows;

	bool activated;
	LineSegment lineSegment;
};

#endif
