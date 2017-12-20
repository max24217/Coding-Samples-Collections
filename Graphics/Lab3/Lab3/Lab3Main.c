//
//  main.c
//  Lab3
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


//Rotation matrix for cube 1
mat4 X_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

//Rotation matrix for cube 4
mat4 FX_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

//Rotation matrix for cube 2
mat4 Y_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};


//Rotation matrix for cube 3
mat4 Z_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};



//The color array will be put into the buffer
vec4 colors[144];

//The actual vector that is going to be put into the  buffer
vec4 vertices[144];

//Our original point (0,0,0,1)
vec4 origin;

//The basic 1X1X1 cube
vec4 cube[36];

//Degree to radian
const GLfloat dToR = M_PI/180;

//Rotation matrix about x-axises in origin
mat4 matrix_X;

//Rotation matrix about y-axises in origin
mat4 matrix_Y;

//Rotation matrix about z-axises in origin
mat4 matrix_Z;

//A faster rotation matrix about x-axises in origin
mat4 matrix_FX;


void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    
    //Initializing the origin
    origin = point4(0.0, 0.0, 0.0, 1.0);
    
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
    
    //Initializing the rotation matrixs
    matrix_X.x = point4(1, 0, 0, 0);
    matrix_X.y = point4(0, cos(dToR*3), sin(dToR*3), 0);
    matrix_X.z = point4(0, -sin(dToR*3), cos(dToR*3), 0);
    matrix_X.w = point4(0, 0, 0, 1);
    
    matrix_Y.x = point4(cos(dToR*3), 0, -sin(dToR*3), 0);
    matrix_Y.y = point4(0, 1, 0, 0);
    matrix_Y.z = point4(sin(dToR*3), 0, cos(dToR*3), 0);
    matrix_Y.w = point4(0, 0, 0, 1);
    
    matrix_Z.x = point4(cos(dToR*3), sin(dToR*3), 0, 0);
    matrix_Z.y = point4(-sin(dToR*3), cos(dToR*3), 0, 0);
    matrix_Z.z = point4(0, 0, 1, 0);
    matrix_Z.w = point4(0, 0, 0, 1);
    
    matrix_FX.x = point4(1, 0, 0, 0);
    matrix_FX.y = point4(0, cos(dToR*8), sin(dToR*8), 0);
    matrix_FX.z = point4(0, -sin(dToR*8), cos(dToR*8), 0);
    matrix_FX.w = point4(0, 0, 0, 1);
    
    
    
    //Putting some random colors for the cubes
    int akb = 0;
    //first cube
    for (; akb < 36; akb ++)
    {
        vertices[akb] = AddiV(cube[akb], point4(-0.5, 0.5, 0.0, 0.0));
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
    }
    
    //Second cube
    for (int k = 0; k < 36; akb ++)
    {
        vertices[akb] = AddiV(cube[k], point4(0.5, 0.5, 0.0, 0.0));
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
        k ++;
    }
    
    //Third cube
    for (int k = 0; k < 36; akb ++)
    {
        vertices[akb] = AddiV(cube[k], point4(-0.5,-0.5, 0.0, 0.0));
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
        k ++;
    }
    
    //Fourth cube
    for (int k = 0; k < 36; akb ++)
    {
        vertices[akb] = AddiV(cube[k], point4(0.5, -0.5, 0.0, 0.0));
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
        k ++;
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
    
    
    ctm_location = glGetUniformLocation(program, "rotation_matrix");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    
    
}//end of init


void display(void)
{
    
    
    glClearDepth(1);
    
    glClearColor (0.0,0.0,0.0,1.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &X_matrix);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &Y_matrix);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 36, 36);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &Z_matrix);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 72, 36);
    
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &FX_matrix);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 108, 36);
    
    
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
    X_matrix = MultiMat4(matrix_X, MultiMat4(translation(0.5, -0.5, 0), X_matrix));
    X_matrix = MultiMat4(translation(-0.5, 0.5, 0), X_matrix);
    
    Y_matrix = MultiMat4(matrix_Y, MultiMat4(translation(-0.5, -0.5, 0.0), Y_matrix));
    Y_matrix = MultiMat4(translation(0.5, 0.5, 0.0), Y_matrix);
    
    Z_matrix = MultiMat4(matrix_Z, MultiMat4(translation(0.5, 0.5, 0), Z_matrix));
    Z_matrix = MultiMat4(translation(-0.5, -0.5, 0), Z_matrix);
    
    FX_matrix = MultiMat4(matrix_FX, MultiMat4(translation(-0.5, 0.5, 0), FX_matrix));
    FX_matrix = MultiMat4(translation(0.5, -0.5, 0.0), FX_matrix);
    
    glutPostRedisplay();
}//end of idle function


int main(int argc, char **argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab3 ZhishengXu");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(
mouse);
    glutIdleFunc(idle);
    
    glutMainLoop();
    
    return 0;
}//end of main






