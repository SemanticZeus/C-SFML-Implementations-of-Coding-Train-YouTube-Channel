#ifndef _MY_WINDOW
#define _MY_WINDOW

#include <SFML/Graphics.hpp>

void initWindow(sf::RenderWindow* win);

inline float map(float value, float start, float end, float map_start, float map_end)
{
    return (value-start)/(end-start) * (map_end-map_start) + map_start;
}

void drawLine(float x1, float y1, float x2, float y2, sf::Color color=sf::Color::White);

int random(int s, int e);

class SliderBar {
public:
    enum Orientation { Vertical, Horizontal };
    void setPosition(double x, double y);
    SliderBar(int _min=1, int _max=100, float _length = 200);
    void draw();
    void update(sf::Event &event);
    float value() { return current; }
    void setValue(float v) { current = v; }
    void setIndexPosition();
    void setStep(float x) { step = x; }
private:
    bool grab;
    float step;
    float diff;
    sf::Vector2f position;
    sf::Vector2f grabPosition;
    Orientation orientation;
    int min,max;
    float current,old_current;
    float length;
    sf::CircleShape index;
    sf::RectangleShape rect, internal_rect;
};

#endif
