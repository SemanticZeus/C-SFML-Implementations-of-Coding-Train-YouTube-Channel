#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <map>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include "window/window.hpp"

int Width = 800;
int Height = 600;
sf::ContextSettings settings(0, 0, 8);
sf::RenderWindow window(sf::VideoMode(Width, Height), sf::String("Fractal Tree Recursive"), sf::Style::Default, settings);

class state {
public:
    double x,y;
    double angle;
};

double len = 100;

int main()
{
    initWindow(&window);
    window.setFramerateLimit(30);

    // L system
    std::string axiom = "F";
    std::string sentence(axiom);
    std::map<char,std::string> rule;
    rule['F']="FF+[+F-F-F]-[-F+F+F]";
    //rule['+']="FF+[+F-FF]";

    auto generate = [&sentence,rule]() {
        std::string new_sentence;
        new_sentence.reserve(sentence.size()*2);
        for (auto c : sentence) {
            std::string a;
            a += c;
            for (auto &m : rule)
                if (m.first == c) {
                    a = m.second;
                    break;
                }
            new_sentence+=a;
        }
        sentence = std::move(new_sentence);
    };

    auto turtle = [&sentence]() {
        state s;
        s.x = 0;
        s.y = 0;
        s.angle = 90;
        std::stack<state> states;
        for (auto &c : sentence) {
            if (c == 'F') {
                double angle = s.angle + rand() % 5;
                double y = s.y + len*std::sin(angle*3.14/180);
                double x = s.x + len*std::cos(angle*3.14/180);
                drawLine(s.x+Width/2, Height - s.y, Width/2 + x, Height - y, sf::Color(rand()%255,rand() % 255, rand() % 255,100));
                s.x = x;
                s.y = y;
            } else if ( c == '+')
                s.angle += 30;
            else if ( c == '-')
                s.angle -= 30;
            else if ( c == '[')
                states.push(s);
            else if ( c == ']') {
                s = states.top();
                states.pop();
            }
        }
    };
    bool redraw = true;
    sf::Clock clock;
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
                redraw = true;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
                generate();
                len*=.8;
                redraw = true;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
                len+=.1;
                redraw = true;
            }  else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) {
                len-=.1;
                redraw = true;
            }

        }
        if (clock.getElapsedTime().asMilliseconds() > 300) {
            clock.restart();
            redraw = true;
        }
        if (redraw) {
            redraw = false;
            window.clear(sf::Color(51,51,51));
            turtle();
        }
        window.display();
    }
    return 0;
}
