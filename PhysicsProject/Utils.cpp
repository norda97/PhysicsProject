#include "Utils.h"

glm::vec2 toScreenSpace(const glm::vec3 & v, sf::RenderWindow & window)
{
	float x = v.x / virtSize.x * (float)window.getSize().x;
	float y = (virtSize.y - v.y) / virtSize.y * (float)window.getSize().y;
	return glm::vec2(x, y);
}

glm::vec2 toScreenSpace(const glm::vec2 & v, sf::RenderWindow & window)
{
	float x = v.x / virtSize.x * (float)window.getSize().x;
	float y = v.y / virtSize.y * (float)window.getSize().y;
	return glm::vec2(x, y);
}

float toScreenSpace(float v, sf::RenderWindow & window, unsigned int sideIndex)
{
	glm::vec2 w(window.getSize().x, window.getSize().y);
	return v / virtSize[sideIndex] * (float)(w[sideIndex]);
}
