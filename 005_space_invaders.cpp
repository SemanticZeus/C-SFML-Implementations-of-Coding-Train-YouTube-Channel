#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

int Width = 800;
int Height = 600;
sf::RenderWindow window(sf::VideoMode(Width, Height), "Space Invaders");

class Drop {
    float x,y;
public:
    Drop(float _x, float _y) : x(_x), y(_y) {}
    void show();
    void move();

    friend class Ship;
};

void Drop::show()
{
    sf::CircleShape circle(8);
    circle.setFillColor(sf::Color(150,0,200));
    circle.setPosition(x,y);
    window.draw(circle);
}
void Drop::move()
{
    y-=5;
}

class Flower {
    float x,y;
    float radius;
public:
    Flower(float _x, float _y, float _radius = 30) 
        : x(_x), y(_y), radius(_radius) {}
    void show();
    void move();
};

void Flower::show()
{
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color(255,0,255));
    circle.setPosition(x-radius,y);
    window.draw(circle);
}

void Flower::move()
{
    
}

class Ship {
    float x;
    std::vector<Drop> drops;
    int dir;
public:
    void setDir(int d) { dir = d; }
    Ship() : dir(0),  x(Width/2) {}
    void fire();
    void show();
    void left() { x-= 5; if (x<0) x=0; }
    void right() { x+=5; if (x>Width-15) x=Width-15; }
};

void Ship::show()
{
    sf::RectangleShape rect(sf::Vector2f(15,40));
    rect.setFillColor(sf::Color::White);
    rect.setPosition(x,Height-40);
    window.draw(rect);
    if (dir>0) right(); else if (dir < 0) left();
    for (auto &d : drops) {
        d.show();
        d.move();
    }
    drops.erase(std::remove_if(drops.begin(),drops.end(),[](auto &d) {return d.y < 0;}),drops.end());
}

void Ship::fire()
{
    drops.emplace_back(x,Height);
}

int main()
{
    window.setFramerateLimit(60);
    Ship ship;
    std::vector<Flower> flowers;
    for (int i=0;i<Width/100;i++)
        flowers.emplace_back(Flower(i*80+100, 80));
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0,0,event.size.width,event.size.height)));
                Height = event.size.height;
                Width = event.size.width;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
                ship.setDir(-1);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
                ship.setDir(1);
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left)
                ship.setDir(0);
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right)
                ship.setDir(0);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                ship.fire();
        }
        window.clear(sf::Color(51,51,51));
        ship.show();
        for (auto flower : flowers)
            flower.show();
        window.display();
    }
}
