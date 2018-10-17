#ifndef UTILS_H
#define UTILS_H

#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"

const glm::vec2 virtSize(10.0f, 10.0f);

glm::vec2 toScreenSpace(const glm::vec3& v, sf::RenderWindow& window);

glm::vec2 toScreenSpace(const glm::vec2& v, sf::RenderWindow& window);

float toScreenSpace(float v, sf::RenderWindow& window, unsigned int sideIndex = 0);

void rotate(glm::vec3& v, const glm::vec3& angle);

#endif
