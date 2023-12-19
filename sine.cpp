#include <SFML/Graphics.hpp>
#include <math.h>

int width = 800;
int height = 600;

sf::RenderWindow *gwindow;

inline float map(float value, float start, float end, float map_start, float map_end)
{
    return (value-start)/(end-start) * (map_end-map_start) + map_start;
}

inline void drawLine(int x1,int y1,int x2, int y2, sf::Color color = sf::Color::White)
{
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(x1,y1);
    line[0].color = color;
    line[1].position = sf::Vector2f(x2,y2);
    line[1].color = color;
    gwindow->draw(line, 2, sf::Lines);
}

void drawSine(double begin, double end, double step, double amp, double f)
{
    double x1=begin,x2,y1,y2;
    y1 = amp*sin(x1*f);
    for (x2=x1+step;x2<=end;x2+=step) {
        y2 = amp*sin(x2*f);
        drawLine(x1,-y1+height/2,x2,-y2+height/2);
        x1 = x2;
        y1 = y2;
    }
    x1 = begin;
    y1 = amp*cos(x1*f);
    for (x2=x1+step;x2<=end;x2+=step) {
        y2 = amp*cos(x2*f);
        drawLine(x1,-y1+height/2,x2,-y2+height/2,sf::Color::Red);
        x1 = x2;
        y1 = y2;
    }
}

int main()
{
    double freq = .01;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Stars", sf::Style::Default, settings);
    gwindow = &window;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                width = event.size.width; height = event.size.height;
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
                freq+=.001;
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
                freq-=.001;

        }
        window.clear(sf::Color::Black);
        drawSine(0,width,2,height/3,freq);
        window.display();
    }
    return 0;
}
