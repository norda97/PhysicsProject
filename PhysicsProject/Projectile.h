#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "glm\glm.hpp"

struct Geometry
{
	enum Type { Sphere, Cuboid } type;
	Geometry(Type type) : type(type) {}
};

struct Sphere : public Geometry
{
	Sphere(float radius) : Geometry(Type::Sphere) { this->radius = radius; }
	float radius;
};

struct Cuboid : public Geometry
{
	Cuboid(const glm::vec3& dim) : Geometry(Type::Cuboid) { this->dim = dim; }
	glm::vec3 dim;
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
	glm::vec3 vel;
	glm::vec3 pos; 
	glm::vec3 angVel;
	float area;
	float mass;
	float cd;
	Geometry* geometry;
};

#endif