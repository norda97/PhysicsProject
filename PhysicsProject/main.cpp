#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <string>

#include "Utils.h"
#include "ObjectManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

	ObjectManager objectManager(window);

	objectManager.addArcher({0.0f, 1.5f, 0.0f});
	// Floor
	objectManager.addRectangle({ virtSize.x*0.5f, -0.1f, 0.0f }, {0.0f, 0.0f, 0.0f}, { virtSize.x*0.5f, 0.1f, virtSize.z }, false);
	// Ceiling
	objectManager.addRectangle({ virtSize.x*0.5f, virtSize.y+0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { virtSize.x*0.5f, 0.1f, virtSize.z }, false);
	// Wall +x
	objectManager.addRectangle({ virtSize.x+0.1f, virtSize.y*0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.1f, virtSize.y*0.5f, virtSize.z*0.5f }, false);
	// Wall -x
	objectManager.addRectangle({ -0.1f, virtSize.y*0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.1f, virtSize.y*0.5f, virtSize.z*0.5f }, false);
	// Wall +z
	objectManager.addRectangle({ virtSize.x*0.5f, virtSize.y*0.5f, virtSize.z*0.5f+0.1f }, { 0.0f, 0.0f, 0.0f }, { virtSize.x*0.5f, virtSize.x*0.5f, 0.1f }, false);
	// Wall -z
	objectManager.addRectangle({ virtSize.x*0.5f, virtSize.y*0.5f, -virtSize.z*0.5f-0.1f }, { 0.0f, 0.0f, 0.0f }, { virtSize.x*0.5f, virtSize.x*0.5f, 0.1f }, false);
	

	objectManager.addBall({ virtSize.x*0.5f, virtSize.y*0.5f, 0.0f }, { 5.f, 0.0f, 0.0f});
	objectManager.addBall({ virtSize.x*0.7f, virtSize.y*0.5f, 0.0f }, { 0.f, 5.0f, 0.0f });

	sf::Clock t;
	float time = 0.0f;
	float dt = 0.0f;
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		t.restart();

		objectManager.updateInput(dt);
		objectManager.update(dt);
		
		window.clear(); 
		objectManager.render();
		window.display();
		dt = t.getElapsedTime().asSeconds();

		time += dt;
		if (time > 1.0f)
		{
			window.setTitle("FPS: " + std::to_string(1.f / dt));
			time = 0.0f;
		}
	}

	return 0;
}