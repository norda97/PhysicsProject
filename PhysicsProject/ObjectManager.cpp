#include "ObjectManager.h"
#include <random>
#include <ctime>

#include "Utils.h"

#define TIME_STEP_FACTOR 1.0f 
#define PI 3.14159265f

ObjectManager::ObjectManager(sf::RenderWindow& window)
{
	this->graphics = new Graphics(&window);
	this->window = &window;

	srand((unsigned int)time(0));
}


ObjectManager::~ObjectManager()
{
	for (Projectile* p : this->projectiles)
		delete p;
	for (Archer* archer : this->archers)
		delete archer;
	for (Geometry* g : this->geometries)
		delete g;
	delete this->graphics;
}

void ObjectManager::addBall(const glm::vec3 & pos, const glm::vec3 & vel, float radius, bool hasPhysics, bool hasCollision)
{
	float r = radius;
	if (r == -1) r = (rand() % 50) / 100.f + 0.2f;
	Sphere* sphere = new Sphere(r);
	sphere->color = sf::Color(rand()% 127+127, rand() % 127 + 127, rand() % 127 + 127, 255);
	Projectile* ball = new Projectile();
	ball->pos = pos;
	ball->vel = vel;
	ball->geometry = sphere;
	ball->hasPhysics = hasPhysics;
	ball->hasCollision = hasCollision;
	ball->area = PI * r*r;
	ball->mass = r * 10 + 100;
	ball->cd = 0.4f;
	this->geometries.push_back(sphere);
	this->projectiles.push_back(ball);
	this->phys.addProjectile(ball);
}

void ObjectManager::addRectangle(const glm::vec3 & pos, const glm::vec3 & vel, const glm::vec3 & size, bool hasPhysics, bool hasCollision)
{
	Cuboid* shape = new Cuboid(size);
	shape->color = sf::Color::Black;
	Projectile* cuboid = new Projectile();
	cuboid->pos = pos;
	cuboid->vel = vel;
	cuboid->geometry = shape;
	cuboid->hasPhysics = hasPhysics;
	cuboid->hasCollision = hasCollision;
	cuboid->area = size.x*size.y;
	cuboid->mass = hasPhysics ? 10 : 1000000.f;
	cuboid->cd = 1.2f;
	this->geometries.push_back(shape);
	this->projectiles.push_back(cuboid);
	this->phys.addProjectile(cuboid);
}

void ObjectManager::addArcher(const glm::vec3 & pos)
{
	this->archers.push_back(new Archer(pos));
}

void ObjectManager::updateInput(float dt)
{
	for (Archer* archer : this->archers)
		archer->processInput(*this->window, this->phys, dt);
}

void ObjectManager::update(float dt)
{
	this->phys.update(dt*TIME_STEP_FACTOR);
}

void ObjectManager::render()
{
	static bool dbg = 0;
	static bool released = false;
	for (unsigned int i = ONE_VIEW; i < 2; i++)
	{
		bool viewXY = (bool)i;
		for (Projectile* p : this->projectiles)
		{
			if (IS_TYPE(p, Cuboid))
				renderCuboid(p, (Cuboid*)p->geometry, viewXY);

			if (IS_TYPE(p, Sphere))
				renderSphere(p, (Sphere*)p->geometry, viewXY);

			// Toggle dbg state
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				released = false;
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && !released)
			{
				dbg ^= 1;
				released = true;
			}

			// Render in dbg if active.
			if (dbg)
				renderDebug(viewXY);
		}
		for (Archer* archer : this->archers)
			renderArcher(archer, viewXY);
	}

	if (ONE_VIEW == false)
	{
		this->graphics->drawLineSegment({ WINDOW_WIDTH / 2.f, 0.0f }, { 0.0f, 1.0f }, WINDOW_HEIGHT);
	}
}

void ObjectManager::renderDebug(bool viewXY)
{
	for (Projectile* p : this->projectiles)
	{	
		glm::vec2 pos = toScreenSpace(p->pos, *window, viewXY);
		glm::vec2 dirVel(p->vel.x, -p->vel[viewXY ? 1 : 2]);
		this->graphics->drawDbArrow(pos, dirVel, 5.0f, 40.f, sf::Color(0x22DD22FF));

		glm::vec2 dirAcc(p->acc.x, -p->acc[viewXY ? 1 : 2]);
		this->graphics->drawDbArrow(pos, dirAcc, 5.0f, 40.f, sf::Color(0x22DD22FF));
	}
}

void ObjectManager::renderCuboid(Projectile * p, Cuboid * shape, bool viewXY)
{
	glm::vec2 pos = toScreenSpace(p->pos, *window, viewXY);
	this->graphics->drawRect(pos, toScreenSpace(shape->dim[0], *window, 0)*2.f,
		toScreenSpace(shape->dim[viewXY ? 1 : 2], *window, viewXY ? 1 : 2)*2.f, shape->color);
}

void ObjectManager::renderSphere(Projectile * p, Sphere * shape, bool viewXY)
{
	float t = 1.0f - abs(p->pos.z) / (virtSize.z*0.5f);
	glm::vec2 pos = toScreenSpace(p->pos, *window, viewXY);
	this->graphics->drawBall(pos, toScreenSpace(shape->radius, *window, 0), mix(shape->color, sf::Color::Black, t), -p->ang[viewXY+1]);
}

void ObjectManager::renderArcher(Archer* archer, bool viewXY)
{
	std::vector<Projectile*>& arrows = archer->getArrows();
	for (Projectile* arrow : arrows)
	{
		LineSegment* lineSegment = (LineSegment*)arrow->geometry;
		glm::vec2 dir(arrow->dir.x, -arrow->dir[viewXY ? 1 : 2]);
		if (arrow->hasPhysics)
		{
			this->graphics->drawLineSegment(toScreenSpace(arrow->pos, *window, viewXY),
				dir, toScreenSpace(lineSegment->d, *window, 0), lineSegment->color);
		}
		else
		{
			this->graphics->drawLineSegment(toScreenSpace(arrow->pos + arrow->parent->pos, *window, viewXY),
				dir, toScreenSpace(lineSegment->d, *window, 0), lineSegment->color);
		}
	}
	glm::vec2 pos = toScreenSpace(archer->getPos(), *window, viewXY);
	this->graphics->drawDbArrow(pos,
		glm::vec2(archer->getDir().x, -archer->getDir()[viewXY?1:2]), 50.f + archer->getX()*100.f, 200.f);
}
