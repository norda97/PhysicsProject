#include "Graphics.h"

Graphics::Graphics(sf::RenderWindow * window)
{
	this->window = window;
}

void Graphics::drawBall(const glm::vec2& pos, float radius, const sf::Color& color)
{
	this->shape.setOrigin(radius, radius);
	this->shape.setFillColor(color);
	this->shape.setRadius(radius);
	this->shape.setPosition(pos.x, pos.y);
	window->draw(this->shape);
}

void Graphics::drawDbArrow(const glm::vec2 & pos, const glm::vec2 & dir, float factor, float max, const sf::Color& color)
{
	float l = glm::length(dir);
	if (l > max) l = max;
	glm::vec2 p2 = pos + glm::normalize(dir)*l*factor;
	sf::Vertex vertices[2] = { sf::Vertex({pos.x, pos.y}), sf::Vertex({p2.x, p2.y}) };
	vertices[0].color = color;
	vertices[1].color = color;
	window->draw(vertices, 2, sf::Lines);
}
