#define _USE_MATH_DEFINES

#include <vector>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>

int width = 1000;
int height = 900;

sf::RenderWindow *window;

double delta_t = 1.0/30.0;
double R0 = 100;

std::vector<double> samples;

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

void drawCircle(double xc, double yc, double r, sf::Color color = sf::Color::Transparent)
{
	sf::CircleShape circle;
	circle.setRadius(r);
	circle.setPosition(xc-r, yc-r);
	circle.setFillColor(color);
	//circle.setOutlineColor(sf::Color::White);
	circle.setOutlineThickness(1);
	window->draw(circle);
}

class Circle {
public:
	Circle(double f = 0, double r=20) : phase(0), radius(r), freq(f) {}
	double phase;
	double radius;
	double freq;
	double xc, yc;
	double xp, yp;

	void update(double x, double y) {
		phase += 2*M_PI*freq*delta_t;
		//if (phase > 2*M_PI) phase -= 2*M_PI;
		xc = x;
		yc = y;
		xp = xc + radius*cos(phase);
		yp = yc + radius*sin(phase);
	}
	void draw() {
		drawCircle(xc, yc, std::abs(radius));
		drawLine(xc, yc, xp, yp);
		double r = 2;
		drawCircle(xp, yp,r, sf::Color::White);
	}
};

std::vector<Circle> circles;

void updateScene()
{
	double xc = 2*R0;
	double yc = height - 2*R0;
	for (auto &c : circles) {
		c.update(xc, yc);
		xc = c.xp;
		yc = c.yp;
	}
	samples.push_back(xc);
}

void drawScene()
{
	for (auto &c : circles) c.draw();
	double xp = circles[circles.size()-1].xp;
	double yp = circles[circles.size()-1].yp;
	double xc = 5*R0;
	double yc;
	for (int i=0;i<samples.size();i++) {
		yc = samples[samples.size()-i-1];
		drawLine(xp, yp, xc, yc);	
		xp = xc;
		yp = yc;
		xc += 5;
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	//settings.majorVersion = 3;
	//settings.minorVersion = 0;
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Fourier Series", sf::Style::Default, settings);
	srand(time(NULL));
	window->setFramerateLimit(60);
	int pause = 0;
	for (int i=0;i<100;i++) {
		double n = i+1;
		double an = sin(M_PI*n/2)/n;
		//double an = -std::pow(-1, n)/n;
		if (std::abs(an) < .00001) continue;
		circles.emplace_back(n*.5, an*R0);
	}
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::Resized) {
				width=event.size.width;
				height=event.size.height;
				window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}
			else if (event.type == sf::Event::MouseMoved) {

			} else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				window->close();
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P)
				pause = 1-pause;
		}

		// clear the window with black color
		if (pause) continue;
		updateScene();
		window->clear(sf::Color::Black);
		drawScene();
		window->display();
	}
	return 0;
}
