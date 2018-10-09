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
		Projectile* p1 = this->projectiles[0];
		Projectile* p2 = this->projectiles[1];
		glm::vec3 loa;
		if (SphereSphereCollision(p1->pos, 0.25f, p2->pos, 0.25f, loa))
		{
			//printf("Collision!\n");
			collisionResponse(*p1, *p2, 0.0f, loa);
		}

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

bool Physics::SphereSphereCollision(glm::vec3 & p1, float r1, glm::vec3 & p2, float r2, glm::vec3& loa)
{
	glm::vec3 p1p2 = p2 - p1;
	const float d = r1 + r2;
	float l = glm::length(p1p2);
	if (l <= d)
	{
		loa = glm::normalize(p1p2);
		p1 += loa * (l - d)*.5f;
		p2 += loa * (d - l)*.5f;
		return true;
	}
	return false;
}

void Physics::collisionResponse(Projectile & p1, Projectile & p2, float e, const glm::vec3 & loa)
{
	float v1rho = glm::dot(loa, p1.vel);
	float v2rho = glm::dot(loa, p2.vel);
	float totalMass = p1.mass + p2.mass;
	
	float A = (p1.mass - e * p2.mass) / totalMass;
	float B = (1.f + e)*p2.mass / totalMass;
	float u1rho = A * v1rho + B * v2rho;

	A = (p2.mass - e * p1.mass) / totalMass;
	B = (1.f + e)*p1.mass / totalMass;
	float u2rho = A * v2rho + B * v1rho;

	// TODO: Add friction and angular velocity.
	p1.vel += (u1rho - v1rho)*loa;
	p2.vel += (u2rho - v2rho)*loa;
}
