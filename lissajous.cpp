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

double delta_t = 1.0/120.0;
double R0 = 200;

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
	Circle(double xx, double yy, double f = 0, double r=20) : phase(0), 
			radius(r), freq(f), xc{xx}, yc{yy} {}
	double phase;
	double radius;
	double freq;
	double xc, yc;
	double xp, yp;

	void update() {
		phase += 2*M_PI*freq*delta_t;
		xp = xc + radius*cos(phase);
		yp = yc + radius*sin(phase);
	}
	void draw() {
		drawCircle(xc, yc, std::abs(radius));
		//drawLine(xc, yc, xp, yp);
		double r = 2;
		drawCircle(xp, yp,r, sf::Color::White);
	}
};


std::vector<Circle> circles;
int num_x;
int num_y;
std::vector<std::vector<sf::VertexArray>> lissajous;
const double freq = .1;
const int radius = 37;
const int margin = 5;

void updateScene()
{
	for (auto &c : circles)
		c.update();
	for (int i=0;i<num_x-1;i++) {
	for (int j=0;j<num_y-1;j++) {
		auto &circle_x = circles[i];
		auto &circle_y = circles[num_x-1+j];
		sf::Vertex point(sf::Vector2f(circle_x.xp, circle_y.yp));
		lissajous[j][i].append(point);
		lissajous[j][i].setPrimitiveType(sf::PrimitiveType::LineStrip);
	}}
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
	for (int i=0;i<num_x-1;i++) for (int j=0;j<num_y-1;j++) 
		window->draw(lissajous[j][i]);
}

void init()
{
	circles.clear();
	lissajous.clear();
	int w = 2*radius+margin;
	num_x = width/w;
	num_y = height/w;
	lissajous.resize(num_y);

	for (int i=1;i<num_x;i++) {
		int xc = w+(i-1)*w+margin+radius;
		int yc = margin+radius;
		circles.emplace_back(xc, yc, i*freq, radius);
	}
	for (int i=1;i<num_y;i++) {
		int xc = margin+radius;
		int yc = w+(i-1)*w+margin+radius;
		circles.emplace_back(xc, yc, i*freq, radius);
		lissajous[i-1].resize(num_x);
	}
	circles.emplace_back(margin+radius, margin+radius, 10, radius);
	std::cout << "lissajous.size = " << lissajous.size() << std::endl;
	std::cout << "lissajous[0].size = " << lissajous[0].size() << std::endl;
	std::cout << "num_x = " << num_x << ", num_y = " << num_y << std::endl;

}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Fourier Series", sf::Style::Default, settings);
	srand(time(NULL));
	window->setFramerateLimit(60);
	int pause = 0;
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
