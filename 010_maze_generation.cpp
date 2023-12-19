#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "window/window.hpp"

int Width = 1500;
int Height = 900;

sf::RenderWindow window(sf::VideoMode(Width, Height), "Maze Generator");

class Cell {
public:
    int i,j;
    bool edges[4]; // u,d,l,r
    bool visited;
    enum Edge { TOP, BOTTOM, LEFT, RIGHT };
    Cell(int x, int y) : i(x), j(y) {
        for (int i=0;i<4;i++) edges[i]=true;
        visited = false;
    }
    void show(float step,float offset=0, bool highlight=false);
};

void Cell::show(float step, float offset, bool highlight)
{
    if (visited) {
        sf::RectangleShape rect(sf::Vector2f(step,step));
        if (highlight) rect.setFillColor(sf::Color(0,50,200,100));
        else rect.setFillColor(sf::Color(255, 0, 255, 100));
        rect.setPosition(offset+i*step, offset+j*step);
        window.draw(rect);
    }
    if (edges[TOP]) drawLine(offset+i*step,offset+j*step,offset+(i+1)*step,offset+j*step);
    if (edges[BOTTOM]) drawLine(offset+i*step,offset+(j+1)*step,offset+(i+1)*step,offset+(j+1)*step);
    if (edges[LEFT]) drawLine(offset+i*step,offset+j*step,offset+i*step,offset+(j+1)*step);
    if (edges[RIGHT]) drawLine(offset+(i+1)*step,offset+j*step,offset+(i+1)*step,offset+(j+1)*step);
}

class Maze {
    std::vector<Cell> cells;
    Cell *current;
    int nx,ny;
    float step;
    float offset;
    enum Direction {TOP=0, BOTTOM, LEFT, RIGHT };
    std::vector<int> stack;
    int si;
public:
    Maze(int x=40, int y=40, float of = 10);
    void show();
    void resize();
    int index(Cell& c, Direction d);
    int randomNeighbor(Cell& c);
    void update();
    void removeWall(Cell& c, Cell& n);
};

void Maze::removeWall(Cell& c, Cell& n)
{
    int x = c.i-n.i;
    int y = c.j-n.j;
    if (x && x > 0) {
        n.edges[Cell::RIGHT]=false;
        c.edges[Cell::LEFT]=false; 
    } else if (x && x < 0) {
        c.edges[Cell::RIGHT]=false;
        n.edges[Cell::LEFT]=false;
    } else if (y && y > 0) {
        c.edges[Cell::TOP]=false;
        n.edges[Cell::BOTTOM]=false;
    } else if (y && y < 0) {
        c.edges[Cell::BOTTOM]=false;
        n.edges[Cell::TOP]=false;
    }
}

void Maze::update()
{
    int n = randomNeighbor(*current);
    while (n<0 && si>=0) {
        current = &cells[stack[si--]];
        //stack.pop_back();
        n = randomNeighbor(*current);
    }
    if (n<0) return;
    //stack.push_back(n);
    stack[++si]=n;
    removeWall(*current, cells[n]);
    current=&cells[n];
    current->visited=true;
}

int Maze::randomNeighbor(Cell& c)
{
    std::vector<int> neighbors;
    neighbors.reserve(4);
    for (int i=0;i<4;i++) {
        int in = index(c, Direction(i));
        if (in < 0) continue;
        if (!cells[in].visited) neighbors.push_back(in);
    }
    if (neighbors.size() == 0) return -1;
    int r = rand() % neighbors.size();
    return neighbors[r];
}

int Maze::index(Cell &c, Direction d)
{
    int i = c.i;
    int j = c.j;
    if (d == TOP) j--;
    else if (d == BOTTOM) j++;
    else if (d == LEFT) i--;
    else if (d == RIGHT) i++;
    if (i>=nx || i < 0 || j>= ny || j < 0) return -1;
    return j*nx + i;
}

Maze::Maze(int x, int y, float of) : nx(x), ny(y), offset(of)
{
    cells.reserve(x*y);
    for (int j=0;j<y;j++)
        for (int i=0;i<x;i++)
            cells.emplace_back(i,j);
    current=&cells[0];
    stack.resize(nx*ny);
    si=0;
    current->visited=true;
    resize();
}

void Maze::resize()
{
    float x =((float)(Width-2*offset)) / nx;
    float y =((float)(Height-2*offset)) / ny;
    step = x > y ? y : x;
}

void Maze::show()
{
    for (auto &c : cells)
        c.show(step,offset);
    current->show(step,offset,true);
}

int main()
{
    int t=0;
    initWindow(&window);
    window.setFramerateLimit(60);
    Maze maze(400,150);
    srand(time(NULL));
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q ) )
                window.close();
            if (event.type == sf::Event::Resized) {
                Width = event.size.width; Height = event.size.height;
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                maze.resize();
            }
        }
        maze.update();
        if (t++ % 200 == 0) {
            window.clear(sf::Color(51,51,51));
            maze.show();
            window.display();
        }
    }
    return 0;
}
