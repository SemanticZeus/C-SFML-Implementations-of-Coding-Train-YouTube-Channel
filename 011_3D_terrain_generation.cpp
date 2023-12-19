#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <GLUT/glut.h>

int width = 800;
int height = 600;
int scale = 1;
int range = 1000;

double camX = 0.0;
double camY = -360.0; //-96.3;
double camZ = 60.0;

double lookAtX = 0.0;
double lookAtY = 0.0;
double lookAtZ = 20.0;

double upX = 0.0;
double upY = 0.0;
double upZ = 1.0;

sf::RenderWindow *window;


inline double map(double value, double start, double end, double map_start, double map_end)
{
	return (value-start)/(end-start) * (map_end-map_start) + map_start;
}

inline void drawLine(double x1,double y1, double z1 ,double x2, double y2, double z2)
{
	sf::Vertex line[2];
	line[0].position = sf::Vector2f(x1,y1);
	line[1].position = sf::Vector2f(x2,y2);
	window->draw(line, 2, sf::Lines);
}

std::vector<std::vector<double>> heights;
void resize()
{
	float ar = (float) width / (float) height;
	glViewport(0,0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ar = 1;
	glFrustum(-ar, ar, -ar, ar, 1, 2000);
	//glOrtho(-range/2, range/2, -range/2, range/2, -20, 1000);
	//glOrtho(-100, 100, -100, 100, 1, 2000);
	glMatrixMode(GL_MODELVIEW);

	heights.clear();
	heights.resize(range);
	std::random_device rd;
	std::mt19937 generator(rd());
	double mean = 10;
	double stddev = 1;
	std::normal_distribution<double> ndist(mean, stddev);
	for (int j=0;j<range;j++) {
		for (int i=0;i<range;i++) {
			double h = std::rand() % 20;
			double x = i-width/scale/2;
			double y = j-height/scale/2;
			x = (i-range/2.0)*scale;
			y = (j-range/2.0)*scale;
			//h = 200*std::exp(std::cos((-x*x-y*y)*2*3.14/1000)/2);
			//h = 200*std::exp((-x*x-y*y)/600);
			//h = 5*sin(10*i*scale*2*3.14/width);
			//x/=8; y/=8; h = x*y*y*y-y*x*x*x;
			//h = 10*std::cos(std::abs(x)+abs(y));
			heights[j].push_back(h);
		}
	}
}

void init()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	window = new sf::RenderWindow(sf::VideoMode(width, height),
		"3D Terrain Generation with Perlin Noise",
		sf::Style::Default, settings);
	srand(time(NULL));
	window->setFramerateLimit(60);
	resize();
}

void update()
{
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
	// clear the window with black color
	window->clear(sf::Color::Black);

	window->setActive();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int cols = range;width/scale;
	int rows = range;height/scale;
	int r = range;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int j=0;j<rows-1;j++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int i=0;i<cols;i++) {
		glColor3f(1.0, 1.0, 1.0);
		double x = (i-range/2.0)*scale*10;
		double y = (j-range/2.0)*scale*10;
		//glVertex3f(map(x*scale, 0, width, -r, r), map(y*scale, 0, height, -r, r), heights[y][x]);
		//glVertex3f(map(x*scale, 0, width, -r, r), map((y+1)*scale, 0, height, -r, r), heights[y+1][x]);
		glVertex3f(x,y, heights[j][i]);
		glVertex3f(x,y+scale*10, heights[j+1][i]);

		//glVertex3f(map((x+1)*scale, 0, width, -1, 1), map(y*scale, 0, height, -1, 1), 0);
	}
	glEnd();
	}
}

int main()
{
	init();
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::Resized) {
				width=event.size.width;
				height=event.size.height;
				window->setView(sf::View(sf::FloatRect(0, 0,
						event.size.width,
						event.size.height)));
				resize();
			}
			else if (event.type == sf::Event::MouseMoved) {
			} else if (event.type == sf::Event::KeyReleased &&
					event.key.code == sf::Keyboard::Escape)
				window->close();
			else if (event.type == sf::Event::KeyPressed) {
				switch(event.key.code) {
					case sf::Keyboard::R:
					camX+=10;
					break;
					case sf::Keyboard::F:
					camX-=10;
					break;
					case sf::Keyboard::Key::Up:
					camY+=10;
					break;
					case sf::Keyboard::Key::Down:
					camY-=10;
					break;
					case sf::Keyboard::Key::Left:
					camZ+=10;
					break;
					case sf::Keyboard::Key::Right:
					camZ-=10;
					break;
					case sf::Keyboard::Q:
					lookAtX+=10;
					break;				
					case sf::Keyboard::A:
					lookAtX-=10;
					break;
					case sf::Keyboard::W:
					lookAtY+=10;
					break;
					case sf::Keyboard::S:
					lookAtY-=10;
					break;
					case sf::Keyboard::E:
					lookAtZ+=10;
					break;
					case sf::Keyboard::D:
					lookAtZ-=10;
					break;
					case sf::Keyboard::P:
					std::cout << "upX,upY,upZ=" << upX << ", " << upY << ", " << upZ << std::endl;
					std::cout << "camX,camY,camZ=" << camX<<", "<<camY<<", "<<camZ<<std::endl;
					std::cout << "lookAtX,Y,Z="<<lookAtX<<", "<<lookAtY<<", "<<lookAtZ<<std::endl;
					break;
	
				}
			}
		}
		update();
		window->display();
	}
	return 0;
}
