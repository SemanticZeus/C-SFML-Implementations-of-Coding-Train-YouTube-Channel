#include <vector>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

int width = 800;
int height = 600;

sf::RenderWindow window(sf::VideoMode(800, 600), "Stars");

inline float map(float value, float start, float end, float map_start, float map_end)
{
    return (value-start)/(end-start) * (map_end-map_start) + map_start;
}

inline void drawLine(int x1,int y1,int x2, int y2)
{
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(x1,y1);
    line[1].position = sf::Vector2f(x2,y2);
    window.draw(line, 2, sf::Lines);
}

struct Star {
    float x,y,z;
    float pz;
    static float speed;
    Star() {
        x = rand() % width;
        y = rand() % height;
        z = rand() % width;
        pz = z;
    }
    void update() {
        pz = z;
        z-=speed;
        if (z<1) {
            x = rand() % width;
            y = rand() % height;
            z = rand() % width;
            pz = z;
        }
    }

    void show() {
        float sx = map((x-width/2)/z,-.5,.5,0,width);
        float sy = map((y-height/2)/z,-.5,.5,0,height);
        float sz = map(z,0,width,5,0);
        float px = map((x-width/2)/pz,-.5,.5,0,width);
        float py = map((y-height/2)/pz,-.5,.5,0,height);
        drawLine(px,py,sx,sy);
    }
};

float Star::speed = 5;

int main()
{
    srand(time(NULL));
    std::vector<Star> stars(25000);
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                width=event.size.width;
                height=event.size.height;
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
            else if (event.type == sf::Event::MouseMoved)
                Star::speed = map(event.mouseMove.x,0,width,0,10);
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        for (auto& star : stars) {
            star.update();
            star.show();
        }
        window.display();
    }
    return 0;
}
