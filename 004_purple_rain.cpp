#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>

const sf::Color Purple(138, 43, 226);
int Width = 640;
int Height = 360;
sf::RenderWindow window(sf::VideoMode(Width, Height), "Purple Rain");

class Drop {
    float x,y,yspeed,length;

public:
    Drop() {
        x = rand() % Width;
        y = rand() % 400 - 400;
        yspeed = rand() % 6 + 4;
        length = random() % 10 + 10;
    }
    void fall();
    void show();
};

void Drop::show() {
    sf::RectangleShape line(sf::Vector2f(2,length));
    line.setPosition(x,y);
    line.setFillColor(Purple);
    window.draw(line);
}

void Drop::fall()
{
    if (y > Height) {
        y = rand() % 100 - 200;
        yspeed = rand() % 6 + 4;
    }
    y+=yspeed;
    yspeed+=0.2;
}



int main()
{
    srand(time(NULL));
    window.setFramerateLimit(60);
    
    std::vector<Drop> rain(500); 
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed 
                    && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::Resized) {
                    Width = event.size.width; Height = event.size.height;
                    window.setView(sf::View(sf::FloatRect(0, 0, Width, Height)));
            }
        }
        window.clear(sf::Color(230, 230, 250));
        for (auto &d : rain) {
            d.fall();
            d.show();
        }
        window.display();
    }
    return 0;
}
