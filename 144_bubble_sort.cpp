#define _USE_MATH_DEFINES

#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <utility>

int width = 1000;
int height = 900;

sf::RenderWindow *window;

std::vector<double> values;

inline double map(double value, double start, double end, double map_start, double map_end)
{
	return (value-start)/(end-start) * (map_end-map_start) + map_start;
}

inline void drawLine(double x1,double y1,double x2, double y2)
{
	sf::Vertex line[2];
	line[0].position = sf::Vector2f(x1,y1);
	line[1].position = sf::Vector2f(x2,y2);
	window->draw(line, 2, sf::Lines);
}

int max = 0;
void updateScene()
{
	if (max == 0 ) return;
	for (int i=0;i<max-1;i++)
		if (values[i]>values[i+1]) std::swap(values[i], values[i+1]);
	max--;
}

void drawScene()
{
	for (int i=0;i<values.size();i++)
		drawLine(i, height, i, height-values[i]);
}

void init()
{
	srand(time(nullptr));
	values.clear();
	values.reserve(width);
	for (int i=0;i<width;i++) {
		values.push_back(float(rand())/float(RAND_MAX) * height);
	}
	max = width-1;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Bubble Sort Visualization", sf::Style::Default, settings);
	srand(time(NULL));
	window->setFramerateLimit(60);
	init();	
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::Resized) {
				width=event.size.width;
				height=event.size.height;
				window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				init();
			}
			else if (event.type == sf::Event::MouseMoved) {

			} else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				window->close();
		}

		// clear the window with black color
		updateScene();
		window->clear(sf::Color::Black);
		drawScene();
		window->display();
	}
	return 0;
}
