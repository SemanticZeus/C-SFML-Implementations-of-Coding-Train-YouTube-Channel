#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>

#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML OpenGL");

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
    
    //prepare OpenGL surface for HSR
    glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar



    bool rotate=true;
    float angle;
    
    // Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window.close();
            if (Event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, Event.size.width, Event.size.height)));
                //glViewport(0, 0, Event.size.width, Event.size.height);
            }

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();

            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
                rotate=!rotate;
            }
 
        }
        
        //Prepare for drawing
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // windowly some transformations for the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        
        if(rotate){
            angle=Clock.getElapsedTime().asSeconds();
        }
        glRotatef(angle * 50, 1.f, 0.f, 0.f);
        //glRotatef(angle * 30, 0.f, 1.f, 0.f);
        //glRotatef(angle * 90, 0.f, 0.f, 1.f);
            
        
         
        //Draw a cube
        glColor3f(0.0,0.0,1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);//draw some squares
//            glColor3i(0,1,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f( 50.f,  50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(0,0,1);
            glVertex3f(-50.f, -50.f, 50.f);
            glVertex3f(-50.f,  50.f, 50.f);
            glVertex3f( 50.f,  50.f, 50.f);
            glVertex3f( 50.f, -50.f, 50.f);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(1,0,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f(-50.f,  50.f,  50.f);
            glVertex3f(-50.f, -50.f,  50.f);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(0,1,0);
            glVertex3f(50.f, -50.f, -50.f);
            glVertex3f(50.f,  50.f, -50.f);
            glVertex3f(50.f,  50.f,  50.f);
            glVertex3f(50.f, -50.f,  50.f);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(1,1,0);
            glVertex3f(-50.f, -50.f,  50.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f,  50.f);
        glEnd();
        glBegin(GL_POLYGON);
//            glColor3f(1,0,0);
            glVertex3f(-50.f, 50.f,  50.f);
            glVertex3f(-50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f,  50.f);

        glEnd();
        window.pushGLStates();
        sf::RectangleShape rect(sf::Vector2f(200,200));
        rect.setFillColor(sf::Color(25,36,89));
        window.draw(rect);
        window.popGLStates();
        // Finally, display rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
