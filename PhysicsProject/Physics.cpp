#include "Physics.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\norm.hpp"

#define IS_TYPE(x, t) (x->geometry->type == Geometry::t)

Physics::Physics()
{
	this->grav = 9.82;
	this->penetrationMomentum = 0.0f;
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
		//Remove if outside screen

		for (unsigned int i = 0; i < this->projectiles.size(); i++)
			updateProjectile(1.f / UPDATE_FREQUENCY * TIME_FACTOR, this->projectiles[i], i);
		
		for (unsigned int i = 0; i < this->projectiles.size(); i++)
		{
			for (unsigned int j = i+1; j < this->projectiles.size(); j++)
			{
				Projectile* p1 = this->projectiles[i];
				Projectile* p2 = this->projectiles[j];
				if (p1->hasPhysics && p2->hasPhysics)
				{
					glm::vec3 loa; // Line of action
					glm::vec3 poc; // Point of collision
					if (IS_TYPE(p1, Sphere) && IS_TYPE(p2, Sphere))
					{
						if (sphereSphereCollision(*p1, *p2, loa, poc))
						{
							printf("Collision [S,S]\n");
							collisionResponse(dt, p1, p2, 1.0f, loa, poc);
						}
					}
					else if ((IS_TYPE(p1, Sphere) && IS_TYPE(p2, Cuboid)) || (IS_TYPE(p1, Cuboid) && IS_TYPE(p2, Sphere)))
					{
						if (IS_TYPE(p2, Sphere))
						{
							Projectile* tmp = p2;
							p2 = p1;
							p1 = tmp;
						}
						if (sphereCuboidCollision(*p1, *p2, loa, poc))
						{
							printf("Collision [S,C]\n");
							collisionResponse(dt, p1, p2, 1.0f, loa, poc);
						}

					}
					else if ((IS_TYPE(p1, Sphere) && IS_TYPE(p2, LineSegment)) || (IS_TYPE(p1, LineSegment) && IS_TYPE(p2, Sphere)))
					{
						if (IS_TYPE(p1, Sphere))
						{
							Projectile* tmp = p1;
							p1 = p2;
							p2 = tmp;
						}
						if (lineSegmentSphereCollision(*p1, *p2, loa, poc))
						{
							printf("Collision [L,S]\n");
							collisionResponseArrowSphere(dt, p1, p2, 0.5f, loa, poc);
						}

					}
				}
			}
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

void Physics::updateProjectile(float dt, Projectile* projectile, unsigned int i)
{
	if (projectile->hasPhysics)
	{
		float fd = calcAirResistence(projectile->cd, projectile->area, projectile->vel);
		glm::vec3 ev(0.0f);
		if (glm::length2(projectile->vel) > EPSILON)
			ev = glm::normalize(projectile->vel);
		glm::vec3 fdv = -fd * ev;
		glm::vec3 fgv = glm::vec3(0.0f, -this->grav * projectile->mass, 0.0f);

		projectile->acc = (fdv + fgv) / projectile->mass;

		projectile->vel += projectile->acc * dt;
		projectile->pos += projectile->vel * dt;
		projectile->ang += projectile->angVel * dt;

		if (glm::length2(projectile->vel) > EPSILON)
			projectile->dir = glm::normalize(projectile->vel);
	}
	static float t = 0.0f;
	t += dt/ TIME_FACTOR;
	if (t > 1.f) 
	{ 
		//printf("[%d]fd: %f, v: %f\n", i, fd, glm::length(projectile->vel));
		t = 0.0f;
	}
}

float Physics::calcAirResistence(float cd, float area, const glm::vec3 & vel)
{
	return 0.5f * AIR_DENSITY * area * cd * glm::length2(vel);
}

glm::vec3 Physics::getClosestPointOBB(const glm::vec3 & p, const glm::vec3 & c, const glm::vec3 & size)
{
	// TODO: Put this in Cuboid and add rotation!!!!!
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

bool Physics::sphereSphereCollision(Projectile& p1, Projectile& p2, glm::vec3& loa, glm::vec3& poc)
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
		poc = p1.pos + loa * ((Sphere*)p1.geometry)->radius;
		return true;
	}
	return false;
}

bool Physics::sphereCuboidCollision(Projectile & sphere, Projectile & cuboid, glm::vec3 & loa, glm::vec3& poc)
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
		poc = getClosestPointOBB(sphere.pos, cuboid.pos, size);
		loa = glm::normalize(poc - sphere.pos);
		return true;
	}
	return false;
}
bool Physics::lineSegmentSphereCollision(Projectile & p1, Projectile & p2, glm::vec3 & loa, glm::vec3 & poc)
{
	LineSegment* lineSegment = (LineSegment*)p1.geometry;
	Sphere* sphere = (Sphere*)p2.geometry;
	glm::vec3 p = p1.pos - p2.pos;
	float d2 = glm::dot(p1.dir, p1.dir);
	float alpha = glm::dot(p, p1.dir) / d2;
	float beta = (glm::dot(p, p) - sphere->radius*sphere->radius) / d2;
	
	float gamma = alpha * alpha - beta;
	if (gamma < 0.0f) return false; // No collision

	float d = -alpha - sqrt(gamma);

	if (d < 0.0f || d >= lineSegment->d) return false;  // Too long or too short.

	poc = p1.pos + p1.dir*d;
	loa = glm::normalize(poc - p2.pos);
	return true;
}
// TODO: Not realistic if mc not align with loa.
void Physics::collisionResponse(float dt, Projectile* p1, Projectile* p2, float e, const glm::vec3 & loa, const glm::vec3& poc)
{
	float v1rho = glm::dot(loa, p1->vel);
	float v2rho = glm::dot(loa, p2->vel);
	float totalMass = p1->mass + p2->mass;
	
	float A = (p1->mass - e * p2->mass) / totalMass;
	float B = (1.f + e)*p2->mass / totalMass;
	float u1rho = A * v1rho + B * v2rho;

	A = (p2->mass - e * p1->mass) / totalMass;
	B = (1.f + e)*p1->mass / totalMass;
	float u2rho = A * v2rho + B * v1rho;

	glm::vec3 preVel1 = p1->vel;
	glm::vec3 preVel2 = p2->vel;

	// TODO: Add friction and angular velocity.
	float dv1 = u1rho - v1rho;
	float dv2 = u2rho - v2rho;
	p1->vel += dv1*loa;
	p2->vel += dv2*loa;

	// Avoid unecessary collision.
	if (!(IS_TYPE(p1, LineSegment) || IS_TYPE(p2, LineSegment)))
	{
		p1->pos += glm::normalize(p1->vel)*0.1f;
		p2->pos += glm::normalize(p2->vel)*0.1f;
	}
	glm::vec3 er = loa;
	glm::vec3 edv = glm::normalize(preVel2-preVel1);
	glm::vec3 edvLoaCross = glm::cross(edv, loa);
	
	// If moving on same line => no angular velocity.
	//if (glm::length2(edvLoaCross) > EPSILON)
	{
		//glm::vec3 en = glm::cross(glm::normalize(edvLoaCross), loa);

		// Angular velocity
		glm::vec3 angAcc1 = getAngularAcc(p1, p2, poc, loa);
		p1->angVel += angAcc1 * dt;

		glm::vec3 angAcc2 = getAngularAcc(p2, p1, poc, loa);
		p2->angVel += angAcc2 * dt;

		/*
		if (IS_TYPE(p1, Sphere))
			p1->angVel += angVelSphere(p1, p2, dv1, 0.1f, er, en);
		else if (IS_TYPE(p1, Cuboid))
			p1->angVel = angVelCuboid(p1, p2, poc, dv1, 0.1f, er, en);

		if (IS_TYPE(p2, Sphere))
			p2->angVel += angVelSphere(p2, p1, dv2, 0.1f, er, -en);
		else if (IS_TYPE(p2, Cuboid))
			p2->angVel = angVelCuboid(p2, p1, poc, dv2, 0.1f, er, -en);
		*/
	}
	//printf("av1: %f, av2: %f\n", p1->angVel.z, p2->angVel.z);
}

void Physics::collisionResponseArrowSphere(float dt, Projectile * p1, Projectile * p2, float e, const glm::vec3 & loa, const glm::vec3 & poc)
{
	float v1rho = glm::dot(loa, p1->vel);
	float v2rho = glm::dot(loa, p2->vel);
	float totalMass = p1->mass + p2->mass;

	float A = (p1->mass - e * p2->mass) / totalMass;
	float B = (1.f + e)*p2->mass / totalMass;
	float u1rho = A * v1rho + B * v2rho;

	A = (p2->mass - e * p1->mass) / totalMass;
	B = (1.f + e)*p1->mass / totalMass;
	float u2rho = A * v2rho + B * v1rho;

	glm::vec3 preVel1 = p1->vel;
	glm::vec3 preVel2 = p2->vel;

	float dv1 = u1rho - v1rho;
	float dv2 = u2rho - v2rho;
	//p1->vel += dv1 * loa;
	p2->vel += dv2 * loa;

	Sphere* sphere = (Sphere*)p2->geometry;
	float vproj = glm::dot(preVel1, loa);
	if (abs(vproj*p1->mass) > penetrationMomentum)
	{
		// Arrow penetrates sphere
		glm::vec3 d = perpDistLineToPoint(p1->pos, p1->dir, p2->pos);
		float angVel = glm::length(preVel1) * (5.f*p1->mass*glm::length(d)) / (2.f*p2->mass*sphere->radius*sphere->radius);
		p2->angVel = angVel*glm::normalize(glm::cross(d, p1->dir));
		p2->addChild(p1);
	}

}

glm::vec3 Physics::perpDistLineToPoint(const glm::vec3 & p, const glm::vec3 & dir, const glm::vec3 & c)
{
	float a = 2.f*glm::dot(p, dir) - glm::dot(dir, c);
	float b = glm::dot(p, p) - glm::dot(p, c);

	float alpha = -a*.5f;
	float gamma = a * a - b;

	if (gamma < 0.0f) return glm::vec3(0.0f);

	float l = alpha - sqrt(gamma);
	return (p+dir*l) - c;
}

glm::vec3 Physics::getAngularAcc(Projectile * p, Projectile * p2, const glm::vec3& poc, const glm::vec3& loa)
{
	// TODO: Calculate the right force. (I don´t know if this is correct)
	float l = glm::length(p2->acc*p2->mass) + glm::length(p->acc*p->mass);
	glm::vec3 force = glm::normalize(p2->acc)*l;
	glm::vec3 forceLoa = glm::dot(force, loa)*loa;
	glm::vec3 r = poc - p->pos;
	// TODO: Make this more pretty.
	if (abs(glm::dot(r, force)) >= glm::length(r)*glm::length(force) - EPSILON)
		return glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 torque = glm::cross(r, force); // length = r*f*sin(angle), r(1,0,0), f(0,-10,0) => positive ang z.
	return torque / getMomemtOfInertia(p);
}

float Physics::getMomemtOfInertia(Projectile * p)
{
	if (IS_TYPE(p, Sphere))
	{
		Sphere* sphere = (Sphere*)p->geometry;
		sphere->inertia = .4f*p->mass*sphere->radius*sphere->radius;
		return sphere->inertia;
	}
	else if (IS_TYPE(p, Cuboid))
	{
		Cuboid* cuboid = (Cuboid*)p->geometry;
		// TODO: FIX THIS so it is not axis aligned.
		float ix = inertiaCuboid(p->mass, cuboid->dim.y, cuboid->dim.z);
		float iy = inertiaCuboid(p->mass, cuboid->dim.x, cuboid->dim.z);
		float iz = inertiaCuboid(p->mass, cuboid->dim.x, cuboid->dim.y);
		cuboid->inertia = glm::vec3(ix, iy, iz);
		return glm::length(cuboid->inertia);
	}
	return 0.0f;
}

glm::vec3 Physics::angVelSphere(Projectile* p, Projectile* p2, float dv, float friction, const glm::vec3& er, const glm::vec3& en)
{
	Sphere* sphere = (Sphere*)p->geometry;
	sphere->inertia = .4f*p->mass*sphere->radius*sphere->radius;
	return angVelCalc(*p, sphere->radius, dv, friction, er, en)/sphere->inertia;
}

glm::vec3 Physics::angVelCuboid(Projectile* p, Projectile* p2, const glm::vec3& poc, float dv, float friction, const glm::vec3& er, const glm::vec3& en)
{

	Cuboid* cuboid = (Cuboid*)p->geometry;
	// TODO: FIX THIS so it is not axis aligned.
	float ix = inertiaCuboid(p->mass, cuboid->dim.y, cuboid->dim.z);
	float iy = inertiaCuboid(p->mass, cuboid->dim.x, cuboid->dim.z);
	float iz = inertiaCuboid(p->mass, cuboid->dim.x, cuboid->dim.y);
	cuboid->inertia = glm::vec3(ix, iy, iz);
	float r = glm::length(poc - p->pos);
	glm::vec3 vel = angVelCalc(*p, r, dv, friction, er, en);
	vel.x /= ix;
	vel.y /= iy;
	vel.z /= iz;
	return vel;
}

float Physics::inertiaCuboid(float m, float a, float b)
{
	return m * (a*a + b*b) / 12.f;
}

glm::vec3 Physics::angVelCalc(Projectile & p, float r, float dv, float friction, const glm::vec3 & er, const glm::vec3 & en)
{
	return (p.mass*r*friction*dv) * glm::cross(en, er);
}
