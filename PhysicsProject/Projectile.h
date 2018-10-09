#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "glm\glm.hpp"

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
};

#endif