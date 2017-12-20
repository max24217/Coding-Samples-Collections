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

//The color array will be put into the buffer
vec4 colors[1520];

//The vector array contains all the vertices
vec4 verticeArray[1520];

//The actual vector that is going to be put into the  buffer
vec4 vertices[1520];

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
   
    //Putting some random colors for the triangles
    for (int akb = 0; akb < g; akb ++)
    {
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
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
    else if (key == 'w')//decreasing the size
    {
        tr_matrix = ScalMultiMat4(1/1.02, tr_matrix);
        tr_matrix.w.w = 1.0;
    }
    else if (key == 's')//increasing the size
    {
        tr_matrix = ScalMultiMat4(1.02, tr_matrix);
        tr_matrix.w.w = 1.0;
    }
    else if (key == 'g')//stop the spinning
    {
        enable = 0;
    }
    
    glutPostRedisplay();
}//end of keyboard


void mouse(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//start to rotate
    {
        oldV = screenToWorldVec4(512, point4(x, y, 0.0, 0.0));
        oldV.z = sqrt(256*256-oldV.y*oldV.y-oldV.x*oldV.x);
        initialP = oldV;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)//decide whether to spin or not
    {
        lastP = screenToWorldVec4(512, point4(x, y, 0.0, 0.0));
        lastP.z = sqrt(256*256-lastP.y*lastP.y-lastP.x*lastP.x);
        
        
        if (distance(initialP, lastP) >= 250)
        {
            enable = 1;
        }
    }
    

}//end of mouse function


void motion(int x, int y)
{
    vec4 newV = screenToWorldVec4(512, point4(x, y, 0.0, 0.0));
    newV.x = -newV.x;
    newV.z = sqrt(256*256-newV.y*newV.y-newV.x*newV.x);
    
    if (newV.x != oldV.x && newV.y != oldV.y && newV.z != oldV.z)
    {
        GLfloat dotR = dotProduct(oldV, newV);
        float radian = acos(dotR/(distance(newV, origin)*distance(oldV, origin)));
        vec4 rotateAixe = crossProduct(newV, oldV);
        
        
        rotateAixe = normalizeVec4(rotateAixe);
        
        float d = sqrt(rotateAixe.y*rotateAixe.y + rotateAixe.z*rotateAixe.z);
        float thetaX = acos(rotateAixe.z/d);
        
        float thetaY = acos(d);
        
        //Creating rotating matrix for y and x axises
        mat4 rotX;
        rotX.x = point4(1.0, 0.0, 0.0, 0.0);
        rotX.y = point4(0.0, rotateAixe.z/d, rotateAixe.y/d, 0.0);
        rotX.z = point4(0.0,-rotateAixe.y/d, rotateAixe.z/d, 0.0);
        rotX.w = point4(0.0, 0.0, 0.0, 1.0);
        
        mat4 rotY;
        rotY.x = point4(d, 0.0, rotateAixe.x, 0.0);
        rotY.y = point4(0.0, 1.0, 0.0, 0.0);
        rotY.z = point4(-rotateAixe.x, 0.0, d, 0.0);
        rotY.w = point4(0.0, 0.0, 0.0, 1.0);
        
        
        
        //Begin to actually create the rotating matrix
        
        mat4 temp;
        if (dotProduct(point4(0.0, 1.0, 0.0, 0.0), rotateAixe) != 0)
        {
            temp = MultiMat4(rotY,rotX);
            temp = MultiMat4(rotat_Z((180/M_PI)*radian), temp);
            temp = MultiMat4(TransposeMat4(rotY), temp);
            temp = MultiMat4(TransposeMat4(rotX), temp);
        }
        else
        {
            temp = MultiMat4(rotat_Z((180/M_PI)*radian), rotY);
            temp = MultiMat4(TransposeMat4(rotY), temp);
        }
        
        
        
        
        /*
        if (oldV.x - newV.x > 0 )
        {
            radian = -radian;
        }
        
        
        mat4 temp;
        //Old way of rotating the sphere
        if (dotProduct(point4(0.0, 1.0, 0.0, 0.0), rotateAixe) != 0)
        {
            temp = MultiMat4(rotat_Y((180/M_PI)*thetaY), rotat_X((180/M_PI)*thetaX));
            temp = MultiMat4(rotat_Z((180/M_PI)*radian), temp);
            temp = MultiMat4(rotat_Y(-(180/M_PI)*thetaY), temp);
            temp = MultiMat4(rotat_X(-(180/M_PI)*thetaX), temp);
        }
        else
        {
            temp = MultiMat4(rotat_Z((180/M_PI)*radian), rotat_Y((180/M_PI)*thetaY));
            temp = MultiMat4(rotat_Y(-(180/M_PI)*thetaY), temp);
        }
        */
        finalTri = temp;
        
        /*
        for (int i = 0; i < g; i ++)
        {
            vertices[i] = pointMulti44Times41(temp, vertices[i]);
            
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
         */
        
        tr_matrix = MultiMat4(temp, tr_matrix);

        oldV = newV;
        
        glutPostRedisplay();
    }
    
    
}//end of motion function




void idle (void)
{
    if (enable)
    {
        /*
        for (int i = 0; i < g; i ++)
        {
            vertices[i] = pointMulti44Times41(finalTri, vertices[i]);
            
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        */
        
        tr_matrix = MultiMat4(finalTri, tr_matrix);
        glutPostRedisplay();
    }
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







