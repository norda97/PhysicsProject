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
			updateProjectile(1.f / UPDATE_FREQUENCY * TIME_FACTOR, this->projectiles[i]);

		Projectile* p1 = this->projectiles[0];
		Projectile* p2 = this->projectiles[1];
		glm::vec3 loa;
		//if(SphereSphereCollision(*p1, 0.25f, *p2, 0.25f, loa))

		if (SphereCuboidCollision(*p1, *p2, loa))
		{
			printf("Collision!\n");
			collisionResponse(*p1, *p2, 1.0f, loa);
		}

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
	glm::vec3 fgv = glm::vec3(0.0f, -this->grav * projectile->mass, 0.0f);

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

glm::vec3 Physics::getClosestPointOBB(const glm::vec3 & p, const glm::vec3 & c, const glm::vec3 & size)
{
	// TODO: Put this in Cuboid!!!
	glm::vec3 right(1.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 forward(0.0f, 0.0f, 1.0f);
	glm::vec3 normals[3] = { right, up, forward };

	glm::vec3 d = p - c;
	glm::vec3 q = c;
	for (unsigned int i = 0; i < 3; i++)
	{
		glm::vec3 extension = normals[i] * size[i];

		float dist = glm::dot(d, normals[i]);
		if (dist > glm::length(extension))
			dist = glm::length(extension);

		if (dist < -glm::length(extension))
			dist = -glm::length(extension);
	
		q += dist * normals[i];
	}
	return q;
}

bool Physics::SphereSphereCollision(Projectile& p1, Projectile& p2, glm::vec3& loa)
{
	glm::vec3 p1p2 = p2.pos - p1.pos;
	const float d = ((Sphere*)p1.geometry)->radius + ((Sphere*)p2.geometry)->radius;
	float l = glm::length(p1p2);
	if (l <= d)
	{
		glm::vec3 u = p2.vel - p1.vel;
		float x = d - l;
		float t = x / glm::length(u);
		p1.pos -= t * p1.vel;
		p2.pos -= t * p2.vel;
		loa = glm::normalize(p2.pos - p1.pos);
		return true;
	}
	return false;
}

bool Physics::SphereCuboidCollision(Projectile & sphere, Projectile & cuboid, glm::vec3 & loa)
{
	
	glm::vec3 right(1.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 forward(0.0f, 0.0f, 1.0f);

	glm::vec3 normals[3] = {right, up, forward};

	glm::vec3 size = ((Cuboid*)cuboid.geometry)->dim;
	float radius = ((Sphere*)sphere.geometry)->radius;

	int collidedWith = 0;
	int lastIndex = -1;
	float minL = 100000.f;
	for (unsigned int i = 0; i < 6; i++)
	{
		glm::vec3 normal = (i > 2 ? -1.f : 1.f) * normals[i%3];
		glm::vec3 pointOnPlane = cuboid.pos + size[i%3]*normal;
		glm::vec3 v = sphere.pos - pointOnPlane;
		float l = glm::dot(normal, v);
		if (l > 0.0)
		{
			v = l * normal;
			if (glm::length(v) <= radius)
			{
				collidedWith++;
				if (abs(l) < minL)
				{
					minL = abs(l);
					lastIndex = i;
				}
			}
		}
		else
		{
			collidedWith++;
			// ?
			if (abs(l) < minL)
			{
				minL = abs(l);
				lastIndex = i;
			}
		}
	}
	if (collidedWith == 6)
	{
		glm::vec3 point = getClosestPointOBB(sphere.pos, cuboid.pos, size);
		loa = glm::normalize(point- sphere.pos);
		return true;
	}
	return false;
}
// TODO: Not working if mc not align with loa.
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

	// Avoid unecessary collision.
	p1.pos += glm::normalize(p1.vel)*0.1f;
	p2.pos += glm::normalize(p2.vel)*0.1f;

	// Angular velocity
	/*float friction = 0.03;
	float radius = 2;
	A = (p1.mass * radius * friction * (u1rho - v1rho)) / 0.4*p1.mass*radius*radius;
	glm::vec3 en = glm::cross(loa, )
	p1.angVel = 
	*/
}
