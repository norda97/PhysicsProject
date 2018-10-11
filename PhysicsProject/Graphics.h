#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"

class Graphics
{
public:
	Graphics(sf::RenderWindow* window);
	void drawBall(const glm::vec2& pos, float radius = 1.0f, const sf::Color& color = sf::Color::White);
	void drawRect(const glm::vec2& pos, float w, float h, const sf::Color& color = sf::Color::White);
	void drawDbArrow(const glm::vec2& pos, const glm::vec2 & dir, float factor, float max, const sf::Color& color = sf::Color::White);

private:
	sf::CircleShape shape;
	sf::RenderWindow* window;
};

#endif
