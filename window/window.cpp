#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "window.hpp"

static sf::RenderWindow *window;

void initWindow(sf::RenderWindow* win)
{
    window = win;
}

int random(int s, int e)
{
    int len = e-s+1;
    return rand() % len + s;
}


void drawLine(float x1, float y1, float x2, float y2, sf::Color color)
{
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(x1,y1);
    line[0].color = color;
    line[1].position = sf::Vector2f(x2,y2);
    line[1].color = color;
    window->draw(line, 2, sf::Lines);
}

SliderBar::SliderBar(int _min, int _max, float _length)
     : grab(false),current(_min), min(_min), max(_max), length(_length), orientation(Horizontal), position(0,0) {
    // external rect
    rect.setSize(sf::Vector2f(length, 20));
    rect.setFillColor(sf::Color(200, 200, 200, 100));
    rect.setPosition(position);
    //internal rect
    internal_rect.setSize(sf::Vector2f(length*.94, 4));
    internal_rect.setFillColor(sf::Color(255,255,255,50));
    internal_rect.setPosition(position.x+length*.03, position.y+8);
    step = map(1,0,max-min,0,internal_rect.getSize().x);
    // index
    index.setRadius(10);
    index.setFillColor(sf::Color(255,255,255));
    //index.setPosition( position.x+length*.03 - 5 + map(current,min,max,0,length-5-length*.03), position.y);
}

void SliderBar::setIndexPosition()
{
    float r = index.getRadius();
    float x = internal_rect.getPosition().x;
    float y = internal_rect.getPosition().y;
    float len = internal_rect.getSize().x;
    float height = internal_rect.getSize().y;
    float dist = map(current, min, max, 0, len);
    index.setPosition(sf::Vector2f(x-r+dist, y-r+height/2));
}

void SliderBar::setPosition(double x, double y)
{
    position.x = x;
    position.y = y;
    rect.setPosition(position);
    internal_rect.setPosition(position.x+length*.03, position.y+8);
}

void SliderBar::draw()
{
    setIndexPosition();
    window->draw(rect);
    window->draw(internal_rect);
    window->draw(index);
}

void SliderBar::update(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        float x = event.mouseButton.x;
        float y = event.mouseButton.y;
        float indexX = index.getPosition().x;
        float indexY = index.getPosition().y;
        float len = 2*index.getRadius();
        if (!grab && x >= indexX && x <= indexX+len && y >= indexY && y <= indexY+len) {
            //current = map(x-position.x+length*.03 - 10/2, 0, length, min, max);
            grab = true;
            grabPosition.x = x;
            grabPosition.y = y;
            old_current = current;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        grab = false;
    } else if (event.type == sf::Event::MouseMoved && grab) {
        diff =  event.mouseMove.x - grabPosition.x;
        diff =  int(map(diff,0,length-5,0,max-min+1)/step)*step;
        current = old_current + diff;
        if (current<min) current = min;
        else if (current > max) current = max;
   }
}

#endif
