// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

// initial point vectors
float x = -1.48;
float y = -1.51;
float z = 2.04;

float dt = 0.5;

// color starting values
float r = 0.3f;
float g = 0.2f;
float bl = 0.1f;

// color deltas
float dr = 0.003f;
float dg = 0.002f;
float dBl = 0.001f;

int i = 0;

// x, y, z,
// x, y, z,
// x, y, z, ...
// std::vector<std::vector<GLfloat> > points; // This is what I was trying, [x,y,z][]
 std::vector<GLfloat> points;
//std::vector<GLubyte> indices;

void halvorsen() {
    float a = 1.89;
    float h = 0.03;

    float dx = (-a * x - 4 * y - 4 * z - (y * y)) * dt;
    float dy = (-a * y - 4 * z - 4 * x - (z * z)) * dt;
    float dz = (-a * z - 4 * x - 4 * y - (x * x)) * dt;
    
    
    x += h * dx;
    y += h * dy;
    z += h * dz;
    
        points.push_back(x);
    points.push_back(y);
    points.push_back(z);
    
    // again, for [x,y,z][] array
//    indices.push_back(i);
//    i = points.size() * 3;
//    newPoint.push_back(x);
//    newPoint.push_back(y);
//    newPoint.push_back(z);
//    points.push_back(newPoint);
}

void incrementColors() {
    if (r + dr >= 1 || r + dr <= 0) {
        dr *= -1;
    }
    if (g + dg >= 1 || g + dg <= 0.1) {
        dg *= -1;
    }
    if (bl + dBl >= 0.9  || bl + dBl <= 0.1) {
        dBl *= -1;
    }
    
    r += dr;
    g += dg;
    bl += dBl;
}

// not actually a lerp I'm pretty sure but a linear translation?
float lerp(float x, float a, float b, float c, float d) {
    return (x - a) / (b - a) * (d - c) + c;
}

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 500, 500, "Tutorial 02 - Red triangle", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // set up the projection matrix (the camera)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // halvorsen
        gluPerspective(45, 500/500, 0.01f, 1000.0f);
    
        gluLookAt(50, 200, 400,
                  0, 0, 0,
                  0, 1, 0);
        glRotatef(-120.0, 0.6, 0.2, 0.7);
        glRotatef(-60.0, 1.0, 0.0, 1.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
    
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    //    glRotatef(80.0, 0.0, 1.0, 1.0);
    //    glTranslatef(0.0, 100.0, 0);
        glScalef(15, 15, 15);
    
    // I haven't normally done this, normally opt to just not wipe the canvas instead
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // I want the drawing to hit the ground running, so I do the maths a few times to get it going,
    // for this I run it 10k times to just get some points to see if the draw works
    for(int i = 0; i < 10000 ; i++) {
        halvorsen(); // pushes a new x, y, and z to `points`
    }
    
//    GLfloat verts[8][3] = {{0.0, 0.0, 0.0},
//               {0.0, 0.0, 0.1},
//               {0.0, 0.1, 0.0},
//               {0.0, 0.1, 0.1},
//               {0.1, 0.0, 0.0},
//               {0.1, 0.0, 0.1},
//               {0.1, 0.1, 0.0},
//               {0.1, 0.1, 0.1}};
//    GLubyte ind[1] = {0};
    
    GLuint vertexbuffer, elementBuffer;
    glGenBuffers(1, &vertexbuffer);
//    glGenBuffers(1, &elementBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(
                     GL_ARRAY_BUFFER,
                     points.size()*sizeof(GLfloat), // sizeof(points) = 24 | points.size() = 30000
                     &points, // pointer to `points`, have tried with `[0]` as well, but neither worked
                     GL_STATIC_DRAW); // static because I've already generated them
    
    
    glEnableClientState(GL_VERTEX_ARRAY);
//        I normally have this outside of the loop because I want to persist the drawings ...
//        but I can't set a BG color now, so I was trying to get the whole std::vector thing
//        to work but ... no luck
//        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    do{
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
        glColor3f(1.0f, 1.0f, 1.0f); // white-ish
        glVertexPointer(
                        3, // this needs to be 1-4, right?
                        GL_FLOAT, // the array is all floats
                        sizeof(GLfloat),
                        0);
        
//        glDrawElements(GL_LINE_STRIP, 1, GL_UNSIGNED_BYTE, 0); // when I was trying with indicies
        glDrawArrays(GL_LINES, 0, points.size());
//        halvorsen();
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
//    glDeleteBuffers(1, &elementBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
