#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include "window/window.hpp"

int Width = 800;
int Height = 600;
sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(Width, Height), sf::String("Fractal Tree Recursive"), sf::Style::Default, settings);

class Branch {
public:
    Branch(float _begin, float _end);
private:
    float begin;
    float end;
};

float step = 0;
void branch(float len, const sf::Vector2f& position, float total_angle)
{
    if (len < 5) return;
    float angle = step - total_angle;
    sf::Vector2f end;
    end.x = len*std::cos(angle*3.14/180) + position.x;
    end.y = len*std::sin(angle*3.14/180) + position.y;
    drawLine(Width/2 + position.x, Height - position.y, Width/2 + end.x, Height - end.y);
    branch(len*.8, end, total_angle+step);
    angle = 180-step-total_angle;
    end.x = len*std::cos(angle*3.14/180) + position.x;
    end.y = len*std::sin(angle*3.14/180) + position.y;
    drawLine(Width/2 + position.x, Height - position.y, Width/2 + end.x, Height - end.y);
    branch(len*.8, end, total_angle-step);
}

int main()
{
    initWindow(&window);
    srand(time(NULL));
    window.setFramerateLimit(60);
    SliderBar sliderBar(0,180,300);
    sliderBar.setPosition(300, Height-50);
    sliderBar.setStep(.1);
    sliderBar.setValue(45);
    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (   event.type == sf::Event::Closed ||
                   (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) )
                window.close();
            if (event.type == sf::Event::Resized) {
                Width = event.size.width; Height = event.size.height;
                window.setView(sf::View(sf::FloatRect(0,0,Width,Height)));
                sliderBar.setPosition(Width-300,Height-50);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V)
                std::cout << sliderBar.value() << std::endl;
            sliderBar.update(event);
        }
        window.clear(sf::Color(51,51,51));
        int len = 250;
        sliderBar.draw();
        drawLine(Width/2,Height,Width/2, Height-len);
        step = sliderBar.value();
        branch(len*.8, sf::Vector2f(0, len), 0);
        window.display();
    }
    return 0;
}
