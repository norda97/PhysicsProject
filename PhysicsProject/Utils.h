#ifndef UTILS_H
#define UTILS_H

#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"

#define ONE_VIEW false
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH (WINDOW_HEIGHT * (ONE_VIEW ? 1 : 2))
#define VIRT_SIZE 10.0f

const glm::vec3 virtSize(VIRT_SIZE, VIRT_SIZE, VIRT_SIZE);

glm::vec2 toScreenSpace(const glm::vec3& v, sf::RenderWindow& window, bool viewXY);

float toScreenSpace(float v, sf::RenderWindow& window, unsigned int sideIndex = 0);

void rotate(glm::vec3& v, const glm::vec3& angle);

float lerp(float a, float b, float t);
sf::Color mix(const sf::Color& a, const sf::Color& b, float t);

#endif
