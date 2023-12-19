#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

int width = 1024;
int height = 900;

class Box {
    float x,y,z;
    float r;

public:
    Box(float x_, float y_, float z_, float r_)
        : x(x_),y(y_),z(z_),r(r_) {}
    void show();
    std::vector<Box> generate();

};


inline int abs(int a)
{
    return a>0 ? a : -a;
}

std::vector<Box> Box::generate()
{
    std::vector<Box> boxes;
    boxes.reserve(27);
    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            for (int k=-1; k<2; k++) {
                int sum = abs(i) + abs(j) + abs(k);
                if (sum <= 1) continue;
                float newR = r/3;
                boxes.emplace_back(Box(x+i*newR, y+j*newR, z+k*newR,newR));
            }
        }
    }
    return boxes;
}

void Box::show()
{
        GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
            {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
            {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
        //Draw a cube
        glColor3f(1,0,0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_POLYGON);//draw some squares
            //glColor3f(1,0,0);
            glNormal3fv(n[5]);
            glVertex3f(x-r/2, y-r/2, z-r/2);
            glVertex3f(x-r/2, y+r/2, z-r/2);
            glVertex3f(x+r/2, y+r/2, z-r/2);
            glVertex3f(x+r/2, y-r/2, z-r/2);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(0,0,1);
            glNormal3fv(n[4]);
            glVertex3f(x-r/2, y-r/2, z+r/2);
            glVertex3f(x-r/2, y+r/2, z+r/2);
            glVertex3f(x+r/2, y+r/2, z+r/2);
            glVertex3f(x+r/2, y-r/2, z+r/2);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(1,0,1);
            glNormal3fv(n[0]);
            glVertex3f(x-r/2, y-r/2, z-r/2);
            glVertex3f(x-r/2, y+r/2, z-r/2);
            glVertex3f(x-r/2, y+r/2, z+r/2);
            glVertex3f(x-r/2, y-r/2, z+r/2);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(0,1,0);
            glNormal3fv(n[2]);
            glVertex3f(x+r/2, y-r/2, z-r/2);
            glVertex3f(x+r/2, y+r/2, z-r/2);
            glVertex3f(x+r/2, y+r/2, z+r/2);
            glVertex3f(x+r/2, y-r/2, z+r/2);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(1,1,0);
            glNormal3fv(n[3]);
            glVertex3f(x-r/2, y-r/2, z+r/2);
            glVertex3f(x-r/2, y-r/2, z-r/2);
            glVertex3f(x+r/2, y-r/2, z-r/2);
            glVertex3f(x+r/2, y-r/2, z+r/2);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(1,0,0);
            glNormal3fv(n[1]);
            glVertex3f(x-r/2, y+r/2, z+r/2);
            glVertex3f(x-r/2, y+r/2, z-r/2);
            glVertex3f(x+r/2, y+r/2, z-r/2);
            glVertex3f(x+r/2, y+r/2, z+r/2);
        glEnd();
}
float xlight,ylight,zlight;
double pos_z = 20;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Menger Sponge");

    std::vector<Box> sponge;
    sponge.emplace_back(0,0,0,150);

    glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    //glEnable(GL_LIGHT1); //Enable light #1
    sf::Clock Clock;
    float angle;

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double ar = 10;
    glFrustum(-ar, ar, -ar, ar, 10, 2000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat lightColor0[] = {1, 1, 1, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, 0.0f, 200.0f, 1.0f}; //Positioned at (4, 0, 8)
    GLfloat lightDirection0[] = {0.0f, 0.0f, -1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection0);


    //gluPerspective(90.f, 1.f, 1.f, 800.0f);//fov, aspect, zNear, zFar

    float zangle=0;
    while (window.isOpen()) {
        sf::Event event;
        while( window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
                zangle+=.5;
            else if (event.type == sf::Event::KeyPressed) {
		switch(event.key.code) {
			case sf::Keyboard::Down:
				pos_z+=10;
				break;
			case sf::Keyboard::Up:
				pos_z-=10;
				break;
		}
	    }
            else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                glViewport(0, 0, event.size.width, event.size.height);
                width = event.size.width;
                height = event.size.height;
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (sponge.size() > 1000)
                    continue;
                std::vector<Box> next;
                next.reserve(sponge.size()*27);
                for (auto &b : sponge) {
                    for (auto &t : b.generate())
                        next.push_back(t);
                }
                sponge = std::move(next);
            }
        }
        // draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	gluLookAt(0, 0, pos_z, 0, 0, 0, 0.0f, 1.0f, 0.0f);

        angle = Clock.getElapsedTime().asSeconds();
        gluLookAt(0, 0, 200, 0, 0, 0, 0.0f, 1.0f, 0.0f);
        //glTranslatef(0,0,-200);
        glRotatef(zangle*10, 0.f, 1.f, 0.f);
        glRotatef(angle*90, 1.f, 0.f, 0.f);
    	GLfloat lightColor0[] = {1, 1, 1, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, 0.0f, 200.0f, 1.0f}; //Positioned at (4, 0, 8)
    GLfloat lightDirection0[] = {0.0f, 0.0f, -1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection0);


        for (auto &box : sponge) {
            box.show();
        }
        window.display();
    }

    return 0;
}
