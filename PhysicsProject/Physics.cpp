#include "Physics.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\norm.hpp"

Physics::Physics()
{
	this->grav = 9.82;
}

Physics::~Physics()
{
}

void Physics::update(float dt)
{
	static float time = 0.0f;
	time += dt;
	if (time > 1.f/UPDATE_FREQUENCY)
	{
		for (unsigned int i = 0; i < this->projectiles.size(); i++)
			updateProjectile(1.f/UPDATE_FREQUENCY * TIME_FACTOR, this->projectiles[i]);

		time = 0.0f;
	}
}

void Physics::addProjectile(Projectile * projectile)
{
	this->projectiles.push_back(projectile);
}

void Physics::applyBowForce(Projectile * projectile, const Bow* bow, const glm::vec3 dir, const float& x)
{
	projectile->vel = sqrt((bow->F * x * bow->e) / (projectile->mass + bow->mass * bow->c)) * glm::normalize(dir);
}

void Physics::updateProjectile(float dt, Projectile* projectile)
{
	float fd = calcAirResistence(projectile->cd, projectile->area, projectile->vel);
	glm::vec3 ev(0.0f);
	if (glm::length2(projectile->vel) > EPSILON)
		ev = glm::normalize(projectile->vel);
	glm::vec3 fdv = -fd * ev;
	glm::vec3 fgv = glm::vec3(0, 0, this->grav * projectile->mass);

	glm::vec3 acc = (fdv + fgv) / projectile->mass;

	projectile->vel += acc * dt;
	projectile->pos += projectile->vel * dt;
	static float t = 0.0f;
	t += dt/ TIME_FACTOR;
	if (t > 1.f) 
	{ 
		printf("fd: %f, v: %f\n", fd, glm::length(projectile->vel));
		t = 0.0f;
	}
}

float Physics::calcAirResistence(float cd, float area, const glm::vec3 & vel)
{
	return 0.5f * AIR_DENSITY * area * cd * glm::length2(vel);
}
