#include "Graphics.h"

Graphics::Graphics(sf::RenderWindow * window)
{
	this->window = window;
}

void Graphics::drawBall(const glm::vec2& pos, float radius, const sf::Color& color, float angle)
{
	this->shape.setOrigin(radius, radius);
	this->shape.setFillColor(color);
	this->shape.setRadius(radius);
	this->shape.setPosition(pos.x, pos.y);
	window->draw(this->shape);
	float x = cos(angle)*radius + pos.x;
	float y = sin(angle)*radius + pos.y;
	sf::Vertex vertices[2] = { sf::Vertex({pos.x, pos.y}), sf::Vertex({x, y}) };
	sf::Color colorInv(sf::Color::White.r-color.r, sf::Color::White.g-color.g, sf::Color::White.b-color.b);
	vertices[0].color = colorInv;
	vertices[1].color = colorInv;
	window->draw(vertices, 2, sf::Lines);
}

void Graphics::drawRect(const glm::vec2 & pos, float w, float h, const sf::Color & color, float angle)
{
	sf::RectangleShape shape(sf::Vector2f(w, h));
	shape.setFillColor(color);
	shape.setOrigin(w*.5f, h*.5f);
	shape.setPosition(pos.x, pos.y);
	shape.setRotation(angle * 180.f/3.1415f);
	window->draw(shape);
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

void Graphics::drawLineSegment(const glm::vec2 & pos, const glm::vec2 & dir, float factor, const sf::Color & color)
{
	float l = glm::length(dir);
	glm::vec2 p2 = pos + glm::normalize(dir)*l*factor;
	sf::Vertex vertices[2] = { sf::Vertex({pos.x, pos.y}), sf::Vertex({p2.x, p2.y}) };
	vertices[0].color = color;
	vertices[1].color = color;
	window->draw(vertices, 2, sf::Lines);
}
