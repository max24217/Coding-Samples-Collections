//
//  main.c
//  Lab5
//
//  Created by XuZhisheng on 11/27/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//
#include "Library.h"
#include "initShader.h"
#include <stdio.h>
#include <time.h>
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

GLuint model_location;
GLuint project_location;

vec4 test;

//Whether we have finished one move for a single plane or not
float count;

//The plane number for current rolling plane
int which;

//The starting position of the first vertice of current plane in the buffer
int where;

//The position of eye
vec4 eyeP;

//The position of the center of the maze
vec4 centerP;

mat4 tr_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

mat4 tr2_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

//Number of sphere vertices in logic
int k = 0;

//Number of sphere vertices that is going to actually put into the buffer
int g = 0;

//The vector array contains all the vertices
vec4 verticeArray[15200];

//The color array will be put into the buffer
vec4 colors[70000];

//The actual vector that is going to be put into the  buffer
vec4 vertices[70000];

//Our original point (0,0,0,1)
vec4 origin;

//The basic 0.5X0.5X0.5 cube
vec4 cube[36];

//The planes 1x1
vec4 plane[36];

//Degree to radian
const GLfloat dToR = M_PI/180;

//Rotation matrix about x-axises in origin
mat4 matrix_X;

//Rotation matrix about y-axises in origin
mat4 matrix_Y;

//Rotation matrix about z-axises in origin
mat4 matrix_Z;

//Rotation matrix for flying around the maze
mat4 flying_Y;

//The total number of vertices of walls and poles in the scene
int numberOfVertice;

//Used to test the frustrum value
float va = 0.0;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    
    numberOfVertice = 0;
    
    //Initializing the origin
    origin = point4(0.0, 0.0, 0.0, 1.0);
    
    //Initializing some variables
    count = 0;
    which = 0;
    where = 0;
    
    test = point4(0, 0, 0, 1.0);
    
    
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
    
    
    //Begin to scale and giving colors to the sphere
    for (int f = 0; f < g; f ++)
    {
        colors[f] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
        vertices[f] = point4(vertices[f].x/4, vertices[f].y/4+0.25, vertices[f].z/4, 1.0);
    }
    //Done with the sphere
    
    numberOfVertice = g;
    where = g;
    //printf("%d\n",numberOfVertice);
    
    //Scalling this cube into a planes
    //In-total we will have 20 planes
    for (int j = 0; j < 20; j ++)
    {
        for (int f = 0; f < 36; f ++)
        {
            plane[f] = point4(cube[f].x*2, 0, cube[f].z*2-j, 1.0);
            vertices[numberOfVertice] = plane[f];
            colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
        }
    }
    
    
    
    
    //Initializing the rotation matrixs on origin
    matrix_X.x = point4(1, 0, 0, 0);
    matrix_X.y = point4(0, cos(dToR*-5), sin(dToR*-5), 0);
    matrix_X.z = point4(0, -sin(dToR*-5), cos(dToR*-5), 0);
    matrix_X.w = point4(0, 0, 0, 1);
    
    matrix_Y.x = point4(cos(dToR*90), 0, -sin(dToR*90), 0);
    matrix_Y.y = point4(0, 1, 0, 0);
    matrix_Y.z = point4(sin(dToR*90), 0, cos(dToR*90), 0);
    matrix_Y.w = point4(0, 0, 0, 1);
    
    matrix_Z.x = point4(cos(dToR*3), sin(dToR*3), 0, 0);
    matrix_Z.y = point4(-sin(dToR*3), cos(dToR*3), 0, 0);
    matrix_Z.z = point4(0, 0, 1, 0);
    matrix_Z.w = point4(0, 0, 0, 1);
    
    //Initilizaing the eyeP and centerP
    eyeP = point4(0, 1, 1, 1.0);
    centerP = point4(0, 0, -1, 1.0);
    
    //Conventions
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
    
    
    model_location = glGetUniformLocation(program, "model_view_matrix");
    project_location = glGetUniformLocation(program, "projection_matrix");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    
    
}//end of init


void display(void)
{
    
    
    glClearDepth(1);
    
    glClearColor (0,0,0,1.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    glUniformMatrix4fv(project_location, 1, GL_FALSE, (GLfloat *) &tr2_matrix);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertice);
    
    glutSwapBuffers();
}//end of display



void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'q' || key == 'Q')
    {
        exit(0);
    }
   
    
}//end of keyboard


void mouse(int button, int state, int x, int y)
{
    
    
    
    
}//end of mouse function





void idle (void)
{

    
    //apply the rotation matrix for all vertices for the sphere
    for (int f = 0; f < g; f ++)
    {
        vertices[f] = point4(vertices[f].x, vertices[f].y-0.25, vertices[f].z, 1.0);
        vertices[f] = pointMulti44Times41(matrix_X, vertices[f]);
        vertices[f] = point4(vertices[f].x, vertices[f].y+0.25, vertices[f].z, 1.0);
    }
    
    
    //Moving all the planes
    for (int f = g; f < numberOfVertice; f ++)
    {
        vertices[f] = point4(vertices[f].x, vertices[f].y, vertices[f].z+0.04, 1.0);
    }
    
    test = point4(0, 0, test.z+0.04, 1.0);
    
    count += 0.04;
    
    //Moving back the position of current rolling plane
    if (count >= 1)
    {
        
        //printVector(test);
        //printf("\n");
        count = 0;
        
        /*
        if (which == 0)
        {
            test = point4(0, 0, test.z-20, 1.0);
        }
        */
        
        for (int f = where; f < (where + 36); f ++)
        {
            vertices[f] = point4(vertices[f].x, vertices[f].y, vertices[f].z-20, 1.0);
        }
        
        which += 1;
        
        where += 36;
        
        //check if current plane is already the last plane
        if (which == 20)
        {
            where = g;
            which = 0;
        }
    }
    
    
    tr_matrix = look_at(eyeP.x, eyeP.y, eyeP.z, centerP.x, centerP.y, centerP.z, 0.0, 1.0, 0.0);
    tr2_matrix = frustum(-0.3, 0.3, -0.3, 0.3, -0.5,-100);
    
    
    //Repush the new modified array back into the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    
    glutPostRedisplay();
}//end of idle function


int main(int argc, char **argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab5 ZhishengXu");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}//end of main

