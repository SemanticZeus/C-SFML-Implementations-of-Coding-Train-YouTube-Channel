#include <vector>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>

int width = 800;
int height = 600;

sf::RenderWindow *window;

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


class Boundary {
public:
	Boundary(double _x1, double _y1, double _x2, double _y2) :
		x1(_x1), x2(_x2), y1(_y1), y2(_y2)
	{}

	void show() {
		drawLine(x1, y1, x2, y2);
	}
	double x1,x2,y1,y2;
};

std::vector<Boundary> walls;

class Ray {
public:
	Ray(double _x, double _y, double angle) : x(_x), y(_y) {
		dx = std::cos(angle);
		dy = std::sin(angle);
	}

	void show();

	void move(double xp, double yp) {
		x = xp;
		y = yp;
	}

	double x,y,dx,dy;
};

struct intersection {
	bool intersect;
	double x,y;
};

auto cast(const Boundary& wall, const Ray& ray) {
	double x1 = wall.x1;
	double x2 = wall.x2;
	double y1 = wall.y1;
	double y2 = wall.y2;

	double x3 = ray.x;
	double y3 = ray.y;
	double x4 = x3+10*ray.dx;
	double y4 = y3+10*ray.dy;

	double den = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
	if (std::abs(den)<.001) return intersection {false, -1, -1};
	double t = ((x1-x3)*(y3-y4) - (y1-y3)*(x3-x4))/den;
	double u = ((x1-x3)*(y1-y2) - (y1-y3)*(x1-x2))/den;
	if (t<0 || t>1 || u<0) return intersection {false, -1, -1};
	return intersection {true, x1+(x2-x1)*t, y1+(y2-y1)*t};
}

void Ray::show()
{
	bool valid = false;
	double xd, yd;
	double distance;
	for (auto &w : walls) {
		auto [v, x1, y1] = cast(w, *this);
		if (!v) continue;
		double d = (x1-x)*(x1-x)+(y1-y)*(y1-y);
		if (!valid) {valid=v; xd=x1; yd=y1; distance=d;}
		else if (distance > d) { xd=x1, yd=y1; distance=d; }
	}
	if (!valid) return;
	drawLine(x,y,xd,yd);
}

#define PI 3.14159265359

class Light {
public:
	Light(): x{}, y{} {
		for (double i=0;i<360;i+=1)
			rays.emplace_back(0,0,i*PI/180);
	}
	void move(double xp, double yp) {
		x = xp;
		y = yp;
		for (auto &r: rays) r.move(x,y);
	}

	void show() {
		double r = 10;
		sf::CircleShape glow(r);
		glow.setPosition(x-r, y-r);
		//glow.setFillColor(sf::Color::Transparent);
		//glow.setOutlineColor(sf::Color::Green);
		window->draw(glow);
		for (auto &ray : rays) ray.show();
	}
	std::vector<Ray> rays;
	double x,y;
};

void init_walls()
{
	walls.clear();
	for (int i=0;i<6;i++) walls.emplace_back(std::rand()%width, std::rand()%height, std::rand()%width, std::rand()%height);
	walls.emplace_back(0,0, width, 0);
	walls.emplace_back(0,0, 0, height);
	walls.emplace_back(width,0, width, height);
	walls.emplace_back(0,height, width, height);

}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "2D Raycasting", sf::Style::Default, settings);
	init_walls();
	srand(time(NULL));
	window->setFramerateLimit(60);
	Light light{};
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::Resized) {
				width=event.size.width;
				height=event.size.height;
				window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				init_walls();
			}
			else if (event.type == sf::Event::MouseMoved) {
				light.move(event.mouseMove.x, event.mouseMove.y);
			} else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				window->close();
		}

		// clear the window with black color
		window->clear(sf::Color::Black);
		for (auto &w : walls) w.show();
		light.show();
		window->display();
	}
	return 0;
}
