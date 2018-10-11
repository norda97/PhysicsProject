#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include "Projectile.h"

#define AIR_DENSITY 1.225f
#define EPSILON 0.00001f
#define UPDATE_FREQUENCY 60.f
#define TIME_FACTOR 0.5f

class Physics
{
public:
	Physics();
	~Physics();

	void update(float dt);

	void addProjectile(Projectile* projectile);

	void applyBowForce(Projectile* projectile, const Bow* bow, const glm::vec3 dir, const float& x);

private:
	void updateProjectile(float dt, Projectile* projectile);
	float calcAirResistence(float cd, float area, const glm::vec3& vel);

	glm::vec3 getClosestPointOBB(const glm::vec3& p, const glm::vec3& c, const glm::vec3& size);

	bool SphereSphereCollision(Projectile& p1, Projectile& p2, glm::vec3& loa);
	bool SphereCuboidCollision(Projectile& sphere, Projectile& cuboid, glm::vec3& loa);

	void collisionResponse(Projectile& p1, Projectile& p2, float e, const glm::vec3& loa);

	std::vector<Projectile*> projectiles;
	float grav;
};

#endif