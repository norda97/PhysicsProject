#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "glm\glm.hpp"

struct Bow
{
	float mb;
	float c;
	float F;
	float e;
};

struct Projectile
{
	glm::vec3 vel;
	glm::vec3 pos; 
	float area;
	float mass;
	float cd;
};

#endif