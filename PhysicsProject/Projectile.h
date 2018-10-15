#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "glm\glm.hpp"

struct Geometry
{
	enum Type { Sphere, Cuboid, LineSegment } type;
	Geometry(Type type) : type(type) {}
};

struct Sphere : public Geometry
{
	Sphere(float radius) : Geometry(Type::Sphere) { this->radius = radius; }
	float radius;
	float inertia;
};

struct Cuboid : public Geometry
{
	Cuboid(const glm::vec3& dim) : Geometry(Type::Cuboid) 
	{ 
		this->dim = dim; 
		this->right = glm::vec3(1.0f, 0.0f, 0.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	glm::vec3 dim;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 inertia;
};

struct LineSegment : public Geometry
{
	LineSegment() : Geometry(Type::LineSegment) {}
	LineSegment(float d, float mcFactor) : Geometry(Type::LineSegment)
	{	
		this->d = d;
		this->mcFactor = mcFactor;
	}
	float mcFactor;
	float d;
};

struct Bow
{
	float mass;
	float c;
	float F;
	float e;
};

struct Projectile
{
	glm::vec3 acc;
	glm::vec3 vel;
	glm::vec3 pos;
	glm::vec3 angVel;
	glm::vec3 ang;
	glm::vec3 dir;
	float area;
	float mass;
	float cd;
	Geometry* geometry;
	bool hasPhysics = true;

	Projectile* parent;
	std::vector<Projectile*> children;

	void addChild(Projectile* p)
	{
		p->hasPhysics = false;
		p->pos = p->pos - this->pos;
		p->parent = this;
		children.push_back(p);
	}

	void updateChildren()
	{
		for (Projectile* child : this->children)
		{

		}
	}
};

#endif