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

void draw_rose(int n, int d)
{
    int mx = width/2;
    int my = height/2;
    int r = width < height ? width/2 - 10 : height/2 - 10;
    double x = mx+r*cos(0);
    double y = my+r*sin(0);
    for (double i=0;i<=360;i+=1) {
        int k = i*d;
        double rr=r*sin(n*k*3.14/180.0);
        double nx = mx+rr*cos(k*3.14/180.0);
        double ny = my+rr*sin(k*3.14/180.0);
        if (i) drawLine(nx, ny, x, y);
        x = nx;y = ny;
    }
}

int main()
{
    initWindow(&window);
    window.setFramerateLimit(60);
    SliderBar sliderBar_a(1,100,300);
    sliderBar_a.setPosition(300, height-50);
    sliderBar_a.setStep(1);
    sliderBar_a.setValue(2);
    SliderBar sliderBar_d(1,180,300);
    sliderBar_d.setPosition(300, height-100);
    sliderBar_d.setStep(1);
    sliderBar_d.setValue(19);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if ( event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
            if (event.type == sf::Event::Resized) {
                width = event.size.width; height = event.size.height;
                window.setView(sf::View(sf::FloatRect(0,0,width,height)));
                sliderBar_a.setPosition(width-300,height-50);
                sliderBar_d.setPosition(width-300,height-100);
            }
            sliderBar_d.update(event);
            sliderBar_a.update(event);
        }
       double p_x = width/2;
        double p_y = height/2;
        window.clear(sf::Color(51,51,51));
        draw_rose(sliderBar_a.value(), sliderBar_d.value());
        //window.draw(&vertex[0], vertex.size(), sf::TriangleFan);
        sliderBar_a.draw();
        sliderBar_d.draw();
 
        window.display();
    }
    return 0;
}
