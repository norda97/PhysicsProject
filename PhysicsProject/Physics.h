#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include "Projectile.h"

#define AIR_DENSITY 1.225f
#define EPSILON 0.00001f
#define UPDATE_FREQUENCY 60.f
#define TIME_FACTOR 0.5f

#define IS_TYPE(x, t) (x->geometry->type == Geometry::t)

class Physics
{
public:
	Physics();
	~Physics();

	void update(float dt);

	void addProjectile(Projectile* projectile);

	void applyBowForce(Projectile* projectile, const Bow* bow, const glm::vec3 dir, const float& x);

private:
	void updateProjectile(float dt, Projectile* projectile, unsigned int i);
	float calcAirResistence(float cd, float area, const glm::vec3& vel);

	glm::vec3 getClosestPointOBB(const glm::vec3& p, const glm::vec3& c, const glm::vec3& size);

	bool sphereSphereCollision(Projectile& p1, Projectile& p2, glm::vec3& loa, glm::vec3& poc);
	bool sphereCuboidCollision(Projectile& sphere, Projectile& cuboid, glm::vec3& loa, glm::vec3& poc);
	bool lineSegmentSphereCollision(Projectile& lineSegment, Projectile& sphere, glm::vec3& loa, glm::vec3& poc);
	bool pointCuboidCollision(const glm::vec3& p, Projectile & cuboid);

	void collisionResponse(float dt, Projectile* p1, Projectile* p2, float e, const glm::vec3& loa, const glm::vec3& poc);
	void collisionResponseArrowSphere(float dt, Projectile* p1, Projectile* p2, float e, const glm::vec3& loa, const glm::vec3& poc);
	glm::vec3 perpDistLineToPoint(const glm::vec3& p, const glm::vec3& dir, const glm::vec3& c);
	
	std::vector<Projectile*> projectiles;
	float grav;
	float penetrationMomentum;
};

#endif