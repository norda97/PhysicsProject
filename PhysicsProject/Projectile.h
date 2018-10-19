#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "glm\glm.hpp"
#include "SFML/Graphics.hpp"

struct Geometry
{
	enum Type { Sphere, Cuboid, LineSegment } type;
	Geometry(Type type) : type(type), color(sf::Color::White) {}
	sf::Color color;
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
	glm::vec3 acc = {0.0f, 0.0f, 0.0f};
	glm::vec3 vel = { 0.0f, 0.0f, 0.0f };
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 angVel = { 0.0f, 0.0f, 0.0f };
	glm::vec3 ang = { 0.0f, 0.0f, 0.0f };
	glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
	float area = 1.f;
	float mass = 1.f;
	float cd = 0.5f;
	Geometry* geometry = nullptr;
	bool hasPhysics = true;
	bool hasCollision = true;

	Projectile* parent = nullptr;
	std::vector<Projectile*> children;

	void addChild(Projectile* p)
	{
		p->hasCollision = false;
		p->hasPhysics = false;
		p->pos = p->pos - this->pos;
		p->parent = this;
		children.push_back(p);
	}
};

#endif