//
//  main.c
//  Project1 and Lab2
//
//  Created by XuZhisheng on 9/28/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//
#include "Library.h"
#include "initShader.h"
#include <stdio.h>
#include <Math.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__


GLuint ctm_location;

mat4 tr_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

float eye_x, eye_y, eye_z;
float at_x,at_y,at_z;
float light_x,light_y,light_z;

//The collection of vectors for a cube 0.5*0.5*0.5
vec4 cube[36];

//The color array will be put into the buffer
vec4 colors[9620];

//The vector array contains all the vertices
vec4 verticeArray[9620];

//The actual vector that is going to be put into the  buffer
vec4 vertices[9620];

//Degrees to Radian
const GLfloat dToR = M_PI/ 180.0;

//Number of sphere vertices in logic
int k = 0;

//Number of sphere vertices that is going to actually put into the buffer
int g = 0;

//First vector from origin to old point
vec4 oldV;

//Our original point (0,0,0,1)
vec4 origin;

//Used to remember our transform matrix for spinning
mat4 finalTri;

//Used to control if we enable spinning
int enable = 0;

//Used to record the initial point for a single click
vec4 initialP;

//Used to record the last point for a single click
vec4 lastP;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    
    //Initializing the origin
    origin = point4(0.0, 0.0, 0.0, 1.0);
    
    
    printf("Please enter an eye point: ");
    scanf("%f %f %f", &eye_x, &eye_y, &eye_z);
    fflush(stdin);
    
    printf("\nPlease enter an at point: ");
    scanf("%f %f %f", &at_x, &at_y, &at_z);
    fflush(stdin);
    
    printf("\nPlease enter a light source point: ");
    scanf("%f %f %f", &light_x, &light_y, &light_z);
    fflush(stdin);
    
    
    
    //Creating the vertices for sphere
    for(float phi = -80.0; phi <= 80.0; phi += 20.0)
    {
        float phir = phi*dToR;
        float phir20 = (phi + 20.0)*dToR;
        for(float theta = -180.0; theta <= 180.0; theta += 20.0)
        {
            float thetar = theta*dToR;
            verticeArray[k] = point4(sin(thetar)*cos(phir),
                                     cos(thetar)*cos(phir), sin(phir),1.0);
            if (k >= 2)
            {
                //Putting the actual vertices into the graph
                vertices[g++] = verticeArray[k-2];
                vertices[g++] = verticeArray[k-1];
                vertices[g++] = verticeArray[k];
            }
            
            k++;
            verticeArray[k] = point4(sin(thetar)*cos(phir20),
                                     cos(thetar)*cos(phir20), sin(phir20),1.0);
            
            if (k >= 2)
            {
                //Putting the actual vertices into the graph
                vertices[g++] = verticeArray[k-1];
                vertices[g++] = verticeArray[k-2];
                vertices[g++] = verticeArray[k];
            }
            
            k++;
        }
    }//end of creating vertices for sphere
    
    
    //creating the shape on the poles
    
    float sin80 = sin(80.0*dToR);
    float cos80 = cos(80.0*dToR);
    
    int cs1566 = 2;//used to control when to put the origin of the sphere into the array
    
    for(float theta = -180.0; theta <= 180.0; theta += 20.0)
    {
        float thetar = theta*dToR;
        
        if (g - 1020 >= 3)
        {
            vertices[g++] = verticeArray[k-2];
            vertices[g++] = point4(sin(thetar)*cos80,
                                   cos(thetar)*cos80, sin80, 1.0);
            vertices[g] = point4(0.0, 0.0, 1.0, 1.0);
            
            
            verticeArray[k++] = verticeArray[k-2];
            verticeArray[k++] = point4(sin(thetar)*cos80,
                                       cos(thetar)*cos80, sin80, 1.0);
            verticeArray[k] = point4(0.0, 0.0, 1.0, 1.0);
        }
        else
        {
            vertices[g] = point4(sin(thetar)*cos80,
                                 cos(thetar)*cos80, sin80, 1.0);
            verticeArray[k] = point4(sin(thetar)*cos80,
                                     cos(thetar)*cos80, sin80, 1.0);
        }
        
        
        cs1566 --;
        
        if (cs1566 == 0)//time to put in the origin for the first group of vertices
        {
            vertices[++g] = point4(0.0, 0.0, 1.0, 1.0);
            verticeArray[++k] = point4(0.0, 0.0, 1.0, 1.0);
        }
        
        g++;
        k++;
    }
    
    
    
    
    cs1566 = 2;
    
    float sin_80 = sin(-80.0*dToR);
    float cos_80 = cos(-80.0*dToR);
    //creating shapes on the other side of the pole of the sphere
    for(float theta = -180.0; theta <= 180.0; theta += 20.0)
    {
        float thetar = theta*dToR;
        
        if (g - 1020 >= 57)
        {
            vertices[g++] = verticeArray[k-2];
            vertices[g++] = point4(sin(thetar)*cos_80,
                                   cos(thetar)*cos_80, sin_80, 1.0);
            vertices[g] = point4(0.0, 0.0, -1.0, 1.0);
            
            
            verticeArray[k++] = verticeArray[k-2];
            verticeArray[k++] = point4(sin(thetar)*cos_80,
                                       cos(thetar)*cos_80, sin_80, 1.0);
            verticeArray[k] = point4(0.0, 0.0, -1.0, 1.0);
        }
        else
        {
            vertices[g] = point4(sin(thetar)*cos_80,
                                 cos(thetar)*cos_80, sin_80, 1.0);
            verticeArray[k] = point4(sin(thetar)*cos_80,
                                     cos(thetar)*cos_80, sin_80, 1.0);
        }
        
        
        cs1566 --;
        //Putting the first three vertex into the array
        if (cs1566 == 0)//time to put in the origin
        {
            vertices[++g] = point4(0.0, 0.0, -1.0, 1.0);
            verticeArray[++k] = point4(0.0, 0.0, -1.0, 1.0);
        }
        
        k++;
        g++;
    }
    
    //scalling and moving the vertices of the sphere to correct location
    for (int k = 0; k < g; k ++)
    {
        vec4 skt = vertices[k];
        skt = point4(skt.x*0.5, skt.y*0.5, skt.z*0.5, 1.0);
        skt = AddiV(skt, point4(-0.5, 0.25, 0.0, 0.0));
        vertices[k] = skt;
    }
    
    
    //Hard coding the basic cube in the origin
    cube[0] = point4(-0.25, -0.25, -0.25, 1.0);
    cube[1] = point4(-0.25, -0.25, 0.25, 1.0);
    cube[2] = point4(-0.25, 0.25, 0.25, 1.0);
    cube[3] = point4(0.25, 0.25, -0.25, 1.0);
    cube[4] = point4(-0.25, -0.25, -0.25, 1.0);
    cube[5] = point4(-0.25, 0.25, -0.25, 1.0);
    cube[6] = point4(0.25, -0.25, 0.25, 1.0);
    cube[7] = point4(-0.25, -0.25, -0.25, 1.0);
    cube[8] = point4(0.25, -0.25, -0.25, 1.0);
    cube[9] = point4(0.25, 0.25, -0.25, 1.0);
    cube[10] = point4(0.25, -0.25, -0.25, 1.0);
    cube[11] = point4(-0.25, -0.25, -0.25, 1.0);
    cube[12] = point4(-0.25, -0.25, -0.25, 1.0);
    cube[13] = point4(-0.25, 0.25, 0.25, 1.0);
    cube[14] = point4(-0.25, 0.25, -0.25, 1.0);
    cube[15] = point4(0.25, -0.25, 0.25, 1.0);
    cube[16] = point4(-0.25, -0.25, 0.25, 1.0);
    cube[17] = point4(-0.25, -0.25, -0.25, 1.0);
    cube[18] = point4(-0.25, 0.25, 0.25, 1.0);
    cube[19] = point4(-0.25, -0.25, 0.25, 1.0);
    cube[20] = point4(0.25, -0.25, 0.25, 1.0);
    cube[21] = point4(0.25, 0.25, 0.25, 1.0);
    cube[22] = point4(0.25, -0.25, -0.25, 1.0);
    cube[23] = point4(0.25, 0.25, -0.25, 1.0);
    cube[24] = point4(0.25, -0.25, -0.25, 1.0);
    cube[25] = point4(0.25, 0.25, 0.25, 1.0);
    cube[26] = point4(0.25, -0.25, 0.25, 1.0);
    cube[27] = point4(0.25, 0.25, 0.25, 1.0);
    cube[28] = point4(0.25, 0.25, -0.25, 1.0);
    cube[29] = point4(-0.25, 0.25, -0.25, 1.0);
    cube[30] = point4(0.25, 0.25, 0.25, 1.0);
    cube[31] = point4(-0.25, 0.25, -0.25, 1.0);
    cube[32] = point4(-0.25, 0.25, 0.25, 1.0);
    cube[33] = point4(0.25, 0.25, 0.25, 1.0);
    cube[34] = point4(-0.25, 0.25, 0.25, 1.0);
    cube[35] = point4(0.25, -0.25, 0.25, 1.0);
    
    
    for (int k = 0; k < 36; k ++)
    {
        vertices[g++] = AddiV(point4(cube[k].x, cube[k].y-0.15, cube[k].z, 1.0), point4(0.5, 0.25, 0.0, 0));
    }
    
    //Putting some random colors for the triangles
    for (int akb = 0; akb < g; akb ++)
    {
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
    }
    
    
    //generating the shadow
    for (int k = 0; k < 1164; k ++)
    {
        
        vertices[g++] = point4(light_x-light_y*((light_x-vertices[k].x)/(light_y-vertices[k].y)), -0.16, light_z-light_y*((light_z-vertices[k].z)/(light_y-vertices[k].y)), 1.0);
    }
    
    
    for (int k = 1164; k < 2328; k ++)
    {
        colors[k] = point4(0, 0, 0, 1.0);
    }
    
    
    
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
    
    
    ctm_location = glGetUniformLocation(program, "model_view");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    
    
}//end of init


void display(void)
{
    
    
    glClearDepth(1);
    
    glClearColor (0.0,1.0,0.0,1.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, g);
    
    glutSwapBuffers();
}//end of display



void keyboard(unsigned char key, int mousex, int mousey)
{
    //For scaling up or down, since Mac can not detect mouse scroll up/down event, I implemented it in keyboard.
    //This is comfired by Dr. Tan that I can do that for the project.
    
    if(key == 'q' || key == 'Q')
        exit(0);
  
    
    glutPostRedisplay();
}//end of keyboard


void mouse(int button, int state, int x, int y)
{
    
    
    
    
}//end of mouse function


void motion(int x, int y)
{
   
        
        glutPostRedisplay();
    
    
    
}//end of motion function




void idle (void)
{

    tr_matrix = look_at(eye_x, eye_y, eye_z, at_x, at_y, at_z, 0, 1, 0);
    glutPostRedisplay();
    
}//end of idle function


int main(int argc, char **argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Sphere  Project1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    
    
    glutMainLoop();
    
    return 0;
}//end of main







