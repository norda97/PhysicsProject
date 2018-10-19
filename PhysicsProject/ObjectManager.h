#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "Projectile.h"
#include "Graphics.h"
#include "Physics.h"
#include "Archer.h"

#include "SFML/Graphics.hpp"

class ObjectManager
{
public:
	ObjectManager(sf::RenderWindow& window);
	~ObjectManager();

	void addBall(const glm::vec3& pos, const glm::vec3& vel = glm::vec3(0.0f), float radius = -1, bool hasPhysics = true, bool hasCollision = true);
	void addRectangle(const glm::vec3& pos, const glm::vec3& vel, const glm::vec3& size, bool hasPhysics = true, bool hasCollision = true);
	void addArcher(const glm::vec3& pos);

	void updateInput(float dt);
	void update(float dt);
	void render();

private:
	void renderDebug(bool viewXY);
	void renderCuboid(Projectile* p, Cuboid* shape, bool viewXY);
	void renderSphere(Projectile* p, Sphere* shape, bool viewXY);
	void renderArcher(Archer* archer, bool viewXY);

	std::vector<Geometry*> geometries;
	std::vector<Projectile*> projectiles;
	std::vector<Archer*> archers;
	
	Graphics* graphics;
	sf::RenderWindow* window;
	Physics phys;
};

#endif

