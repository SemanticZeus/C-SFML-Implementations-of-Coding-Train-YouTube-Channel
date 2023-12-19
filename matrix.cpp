#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

int Width = 800;
int Height = 600;
int fontSize = 17;
sf::RenderWindow window(sf::VideoMode(Width, Height), "Matrix Rain");
sf::Font font;

struct JString {
    wchar_t ch;
    float x,y;
    int len;
    std::wstring jstring;
    int speed;

    JString();
    void fall();
    void update();
    void draw();
    void reduce_speed();
    void increase_speed();
    void random_speed();
};

void JString::reduce_speed()
{
    if (speed > 2)
        speed--;
}

void JString::increase_speed()
{
    if (speed < 15)
        speed++;
}

void JString::random_speed()
{
    speed = rand() % 5 + 3;
}

int prev_x = 0;

JString::JString()
{
    x = prev_x;
    prev_x += fontSize;
    y = -(rand() % 500);
    speed = rand() % 5 + 3;
    len = rand() % 15 + 5; 
    for (int i = 0;i<len;i++)
        jstring+=wchar_t(0x30A0 + rand() % 96);
}

void JString::update()
{
    int n = rand() % len;
    
    for (int i=0;i<n;i++)
    {
        int m = rand() % len;
        jstring[m] = wchar_t(0x30A0 + rand() % 96);
    }
}

void JString::fall()
{
    y+=speed;
    if (y>Height) {
        y=0;-(rand() % 500) - 200;
        //speed = rand() % 5 + 4;
    }
}

void JString::draw()
{
    float h = y;
    sf::Text text(L"", font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color(0, 255, 150));

    for (wchar_t jch : jstring) {
        text.setString(jch);
        sf::Glyph glyph = font.getGlyph(jch, fontSize, false);
        h += glyph.advance;
        if (h>=Height) h-=Height;
        text.setPosition(x,h);
        window.draw(text);
    }
}

int main()
{
    font.loadFromFile("Osaka.ttf");
    window.setFramerateLimit(60);
    sf::Clock timer;
    srand(time(NULL));
    font.loadFromFile("Osaka.ttf");
    std::vector<JString> matrix(Width/fontSize);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                Height = event.size.height;
                if (event.size.width < Width) {
                    for (auto it = matrix.begin();it!=matrix.end();) {
                        if (it->x > event.size.width)
                            it = matrix.erase(it);
                        else
                            it++;
                    }
                } else if (event.size.width > matrix.back().x+3*fontSize) {
                    prev_x = matrix.back().x + fontSize;
                    while (prev_x < event.size.width - fontSize) {
                        matrix.push_back(JString());
                    }
                }
                Width = event.size.width;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
                for (auto &js : matrix)
                    js.reduce_speed();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
                for (auto &js : matrix)
                    js.increase_speed();
           if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
                for (auto &js : matrix)
                    js.random_speed();

        }
        if (timer.getElapsedTime() >= sf::milliseconds(100)) {
            for (auto &jstring : matrix)
                jstring.update();
            timer.restart();
        }
        window.clear(sf::Color::Black);
        for (auto &jstring : matrix) {
            jstring.draw();
            jstring.fall();
        }
        window.display();
    }
    return 0;
}
