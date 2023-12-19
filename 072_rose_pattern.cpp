#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "window/window.hpp"

int width = 800;
int height = 600;
sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(width, height), sf::String("Fractal Tree Recursive"), sf::Style::Default, settings);

int main()
{
    initWindow(&window);
    srand(time(NULL));
    window.setFramerateLimit(60);
    sf::ConvexShape convex;
    int count = 2*M_PI / .02;
    convex.setPointCount(count);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if ( event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) )
            {
                window.close();
            }
            if (event.type == sf::Event::Resized) {
                width = event.size.width; height = event.size.height;
                window.setView(sf::View(sf::FloatRect(0,0,width,height)));
            }
        }
        double p_x = width/2;
        double p_y = height/2;
        window.clear(sf::Color(51,51,51));
        for (int i=0;i<count;i++) {
            double a = 0.0 + i*.02;
            double r = 200 * std::cos(4 * a);
            double x = r * std::cos(a) + width/2;
            double y = r * std::sin(a) + height /2;
            convex.setPoint(i,sf::Vector2f(x,y));
        }
        convex.setFillColor(sf::Color::Green);
        convex.setOutlineColor(sf::Color::Red);
 
        /*for (double a = 0.0; a < 2*M_PI; a+=0.01) {
            double r = 400 * std::cos(7 * a);
            double x = r * std::cos(a) + width/2;
            double y = r * std::sin(a) + height /2;
            drawLine(p_x,p_y,x,y);
            p_x=x; p_y=y;
        }*/
        window.draw(convex);
        //window.draw(&vertex[0], vertex.size(), sf::TriangleFan);
        window.display();
    }
    return 0;
}
