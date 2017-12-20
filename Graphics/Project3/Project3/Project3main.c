//
//  main.c
//  Project 3
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


//The variables used to pass values into the shader file
GLuint model_location;
GLuint project_location;
GLuint ambinent_P;
GLuint diffuse_P;
GLuint specular_P;
GLuint light_Position;
GLuint shiness;
GLuint atten_Constant;
GLuint atten_linear;
GLuint atten_quatric;
GLuint blackC;
GLuint isS;

//The variables that is actually passed into the buffer
vec4 amb_P;
vec4 diffu_P;
vec4 spec_P;
vec4 light_P;
vec4 black_Color;
GLfloat shin;
GLfloat att_C = 0;
GLfloat att_L = 0.8;
GLfloat att_Q = 0.0;

//The center point of red ball
vec4 center_Red;

//The center point of green ball
vec4 center_Green;

//The center point of blue ball
vec4 center_Blue;

//The center point of yellow ball
vec4 center_Yellow;

//The center point of brown ball
vec4 center_Brown;

//Whether we enable the rotation of balls or not
int enable = 0;

//The position of the first vertex of the light source white ball in the buffer
int where_light = 5 * 1128;

//The position of the first vertex of the brown ball in the buffer
int where_brown = 4 * 1128;

//The position of the first vertex of the yellow ball in the buffer
int where_yellow = 3 * 1128;

//The position of the first vertex of the blue ball in the buffer
int where_blue = 2 * 1128;

//The position of the first vertex of the green ball in the buffer
int where_green = 1 * 1128;

//The position of the first vertex of the red ball in the buffer
int where_red = 0;

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

//light source attributes
vec4 light_diffuse;
vec4 light_specular;
vec4 light_ambient;

//The array for all the materials for each objects
material materials[7];

//Number of sphere vertices in logic
int k = 0;

//Number of sphere vertices that is going to actually put into the buffer
int g = 0;

//The vector array contains all the vertices
vec4 verticeArray[1520];

//The vNormal array will be put into the buffer
vec4 normals[10000];

//The actual vector that is going to be put into the  buffer
vec4 vertices[10000];

//Our original point (0,0,0,1)
vec4 origin;

//The basic 0.5X0.5X0.5 cube
vec4 cube[36];

//The planes 20x20
vec4 plane[36];

//Degree to radian
const GLfloat dToR = M_PI/180;

//Rotation matrix about x-axises in origin
mat4 matrix_X;

//Rotation matrix about y-axises in origin
mat4 matrix_Y;

//Rotation matrix about z-axises in origin
mat4 matrix_Z;

//Rotation matrix about x-axiese in origin in negative direction
mat4 matrix_XN;

//Rotation matrix about y-axiese in origin in negative direction
mat4 matrix_YN;

//Rotation matrix for green ball
mat4 matrix_green;

//Rotation matrix for blue ball
mat4 matrix_blue;

//Rotation matrix for yellow ball
mat4 matrix_yellow;

//Rotation matrix for brown ball
mat4 matrix_brown;

//Rotation matrix for flying around the maze
mat4 flying_Y;

//The total number of vertices of walls and poles in the scene
int numberOfVertice;


void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    
    
    //initializing some variables
    numberOfVertice = 0;
    light_ambient = point4(0.2, 0.2, 0.2, 1);
    light_diffuse = point4(1, 1, 1, 1);
    light_specular = point4(1, 1, 1, 1);
    
    //The red ball
    materials[0].reflect_ambient = point4(1, 0, 0, 1);
    materials[0].reflect_diffuse = point4(1, 0, 0, 1);
    materials[0].reflect_specular = point4(1, 1, 1, 1);
    materials[0].shininess = 10;
    
    //The green ball
    materials[1].reflect_ambient = point4(0, 1, 0, 1);
    materials[1].reflect_diffuse = point4(0, 1, 0, 1);
    materials[1].reflect_specular = point4(1, 1, 1, 1);
    materials[1].shininess = 10;
    
    //The blue ball
    materials[2].reflect_ambient = point4(0, 0, 1, 1);
    materials[2].reflect_diffuse = point4(0, 0, 1, 1);
    materials[2].reflect_specular = point4(1, 1, 1, 1);
    materials[2].shininess = 10;
    
    //The yellow ball
    materials[3].reflect_ambient = point4(1, 1, 0, 1);
    materials[3].reflect_diffuse = point4(1, 1, 0, 1);
    materials[3].reflect_specular = point4(1, 1, 1, 1);
    materials[3].shininess = 10;
    
    //The brown ball
    materials[4].reflect_ambient = point4(1, 0.5, 0, 1);
    materials[4].reflect_diffuse = point4(1, 0.5, 0, 1);
    materials[4].reflect_specular = point4(1, 1, 1, 1);
    materials[4].shininess = 10;
    
    //The light ball, light source
    materials[5].reflect_ambient = point4(0.3, 0.3, 0.3, 1);
    materials[5].reflect_diffuse = point4(1, 1, 1, 1);
    materials[5].reflect_specular = point4(1, 1, 1, 1);
    materials[5].shininess = 20;
    
    //The plane
    materials[6].reflect_ambient = point4(0, 1, 0, 1);
    materials[6].reflect_diffuse = point4(0, 1, 0, 1);
    materials[6].reflect_specular = point4(1, 1, 1, 1);
    materials[6].shininess = 2;
    
    //Initializing the start position for light source
    light_P = point4(0, 3, 0, 1.0);
    
    
    //Initializing the origin
    origin = point4(0.0, 0.0, 0.0, 1.0);
    
    //The shadow color
    black_Color =  point4(0, 0, 0, 1);
    
    //The center points of all five color balls
    center_Red =  point4(0, 0.5, 0, 1.0);
    center_Green =  point4(1, 0.5, 0, 1.0);
    center_Blue = point4(2, 0.5, 0, 1);
    center_Yellow =  point4(3, 0.5, 0, 1);
    center_Brown = point4(4, 0.5, 0, 1);
    
    
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
    
    
    //creating six spheres
    for (int akb = 0; akb < 6; akb ++)
    {
        
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
                    vertices[g+numberOfVertice] = verticeArray[k-2];
                    g ++;
                    vertices[g+numberOfVertice] = verticeArray[k-1];
                    g ++;
                    vertices[g+numberOfVertice] = verticeArray[k];
                    g ++;
                }
                
                k++;
                verticeArray[k] = point4(sin(thetar)*cos(phir20),
                                         cos(thetar)*cos(phir20), sin(phir20),1.0);
                
                if (k >= 2)
                {
                    //Putting the actual vertices into the graph
                    vertices[g+numberOfVertice] = verticeArray[k-1];
                    g ++;
                    vertices[g+numberOfVertice] = verticeArray[k-2];
                    g ++;
                    vertices[g+numberOfVertice] = verticeArray[k];
                    g ++;
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
                vertices[g+numberOfVertice] = verticeArray[k-2];
                g ++;
                vertices[g+numberOfVertice] = point4(sin(thetar)*cos80,
                                       cos(thetar)*cos80, sin80, 1.0);
                g ++;
                vertices[g+numberOfVertice] = point4(0.0, 0.0, 1.0, 1.0);
                
                
                verticeArray[k++] = verticeArray[k-2];
                verticeArray[k++] = point4(sin(thetar)*cos80,
                                           cos(thetar)*cos80, sin80, 1.0);
                verticeArray[k] = point4(0.0, 0.0, 1.0, 1.0);
            }
            else
            {
                vertices[g+numberOfVertice] = point4(sin(thetar)*cos80,
                                     cos(thetar)*cos80, sin80, 1.0);
                verticeArray[k] = point4(sin(thetar)*cos80,
                                         cos(thetar)*cos80, sin80, 1.0);
            }
            
            
            cs1566 --;
            
            if (cs1566 == 0)//time to put in the origin for the first group of vertices
            {
                g ++;
                vertices[g+numberOfVertice] = point4(0.0, 0.0, 1.0, 1.0);
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
                vertices[g+numberOfVertice] = verticeArray[k-2];
                g ++;
                vertices[g+numberOfVertice] = point4(sin(thetar)*cos_80,
                                       cos(thetar)*cos_80, sin_80, 1.0);
                g ++;
                vertices[g+numberOfVertice] = point4(0.0, 0.0, -1.0, 1.0);
                
                
                verticeArray[k++] = verticeArray[k-2];
                verticeArray[k++] = point4(sin(thetar)*cos_80,
                                           cos(thetar)*cos_80, sin_80, 1.0);
                verticeArray[k] = point4(0.0, 0.0, -1.0, 1.0);
            }
            else
            {
                vertices[g+numberOfVertice] = point4(sin(thetar)*cos_80,
                                     cos(thetar)*cos_80, sin_80, 1.0);
                verticeArray[k] = point4(sin(thetar)*cos_80,
                                         cos(thetar)*cos_80, sin_80, 1.0);
            }
            
            
            cs1566 --;
            //Putting the first three vertex into the array
            if (cs1566 == 0)//time to put in the origin
            {
                g ++;
                vertices[g+numberOfVertice] = point4(0.0, 0.0, -1.0, 1.0);
                verticeArray[++k] = point4(0.0, 0.0, -1.0, 1.0);
            }
            
            k++;
            g++;
        }
        
        
        
        
        //The red ball
        if (akb == 0)
        {
            //Begin to scale the sphere
            for (int f = numberOfVertice; f < (g+numberOfVertice); f ++)
            {
                vertices[f] = point4(vertices[f].x/2, vertices[f].y/2+0.5, vertices[f].z/2, 1.0);
            }
        }
        else if (akb == 1)//the green ball
        {
            for (int f = numberOfVertice; f < (g+numberOfVertice); f ++)
            {
                vertices[f] = point4(vertices[f].x/2+1, vertices[f].y/2+0.5, vertices[f].z/2, 1.0);
            }
        }
        else if (akb == 2)//the blue ball
        {
            for (int f = numberOfVertice; f < (g+numberOfVertice); f ++)
            {
                vertices[f] = point4(vertices[f].x/2+2, vertices[f].y/2+0.5, vertices[f].z/2, 1.0);
            }
        }
        else if (akb == 3)//the yellow ball
        {
            for (int f = numberOfVertice; f < (g+numberOfVertice); f ++)
            {
                vertices[f] = point4(vertices[f].x/2+3, vertices[f].y/2+0.5, vertices[f].z/2, 1.0);
            }
        }
        else if (akb == 4)//the brown ball
        {
            for (int f = numberOfVertice; f < (g+numberOfVertice); f ++)
            {
                vertices[f] = point4(vertices[f].x/2+4, vertices[f].y/2+0.5, vertices[f].z/2, 1.0);
            }
        }
        else if (akb == 5)//the white ball, the light source
        {
            for (int f = numberOfVertice; f < (g+numberOfVertice); f ++)
            {
                vertices[f] = point4(vertices[f].x/8, vertices[f].y/8+3, vertices[f].z/8, 1.0);
            }
        }
        
        
        numberOfVertice += g;
        g = 0;
        k = 0;
        
    }//end of creating six spheres
    
    
    //Putting in the normals of each point
    int joker = 0;
    //First our red ball
    for (int f = joker; f < (joker+1128); f ++)
    {
        normals[f] = normalizeVec4(SubV(vertices[f], center_Red));
    }
    
    joker += 1128;
    
    //Next is the green ball
    for (int f = joker; f < (joker+1128); f ++)
    {
        normals[f] = normalizeVec4(SubV(vertices[f], center_Green));
    }
    
    joker += 1128;
    
    //Next is the blue ball
    for (int f = joker; f < (joker+1128); f ++)
    {
        normals[f] = normalizeVec4(SubV(vertices[f], center_Blue));
    }
    
    joker += 1128;
    
    //Next is the yellow ball
    for (int f = joker; f < (joker+1128); f ++)
    {
        normals[f] = normalizeVec4(SubV(vertices[f], center_Yellow));
    }
    
    joker += 1128;
    
    //Next is the brown ball
    for (int f = joker; f < (joker+1128); f ++)
    {
        normals[f] = normalizeVec4(SubV(vertices[f], center_Brown));
    }
    
    joker += 1128;
    //Done with the spheres
    
    //Scalling this cube into a plane and putting it into the buffer
    for (int f = 0; f < 36;f ++)
    {
        plane[f] = point4(cube[f].x*40, 0, cube[f].z*40, 1.0);
        vertices[numberOfVertice+f] = plane[f];
        normals[numberOfVertice+f] = point4(0, 1, 0, 0);
    }
    
    numberOfVertice += 36;
    
    //Initializing the rotation matrixs on origin
    matrix_X.x = point4(1, 0, 0, 0);
    matrix_X.y = point4(0, cos(dToR*-3), sin(dToR*-3), 0);
    matrix_X.z = point4(0, -sin(dToR*-3), cos(dToR*-3), 0);
    matrix_X.w = point4(0, 0, 0, 1);
    
    
    matrix_XN.x = point4(1, 0, 0, 0);
    matrix_XN.y = point4(0, cos(dToR*3), sin(dToR*3), 0);
    matrix_XN.z = point4(0, -sin(dToR*3), cos(dToR*3), 0);
    matrix_XN.w = point4(0, 0, 0, 1);
    
    
    matrix_Y.x = point4(cos(dToR*-3), 0, -sin(dToR*-3), 0);
    matrix_Y.y = point4(0, 1, 0, 0);
    matrix_Y.z = point4(sin(dToR*-3), 0, cos(dToR*-3), 0);
    matrix_Y.w = point4(0, 0, 0, 1);
    
    matrix_YN.x = point4(cos(dToR*3), 0, -sin(dToR*3), 0);
    matrix_YN.y = point4(0, 1, 0, 0);
    matrix_YN.z = point4(sin(dToR*3), 0, cos(dToR*3), 0);
    matrix_YN.w = point4(0, 0, 0, 1);
    
    matrix_green.x = point4(cos(dToR*-1), 0, -sin(dToR*-1), 0);
    matrix_green.y = point4(0, 1, 0, 0);
    matrix_green.z = point4(sin(dToR*-1), 0, cos(dToR*-1), 0);
    matrix_green.w = point4(0, 0, 0, 1);
    
    matrix_blue.x = point4(cos(dToR*-2), 0, -sin(dToR*-2), 0);
    matrix_blue.y = point4(0, 1, 0, 0);
    matrix_blue.z = point4(sin(dToR*-2), 0, cos(dToR*-2), 0);
    matrix_blue.w = point4(0, 0, 0, 1);
    
    matrix_yellow.x = point4(cos(dToR*-3), 0, -sin(dToR*-3), 0);
    matrix_yellow.y = point4(0, 1, 0, 0);
    matrix_yellow.z = point4(sin(dToR*-3), 0, cos(dToR*-3), 0);
    matrix_yellow.w = point4(0, 0, 0, 1);
    
    matrix_brown.x = point4(cos(dToR*-4), 0, -sin(dToR*-4), 0);
    matrix_brown.y = point4(0, 1, 0, 0);
    matrix_brown.z = point4(sin(dToR*-4), 0, cos(dToR*-4), 0);
    matrix_brown.w = point4(0, 0, 0, 1);
    
    
    matrix_Z.x = point4(cos(dToR*3), sin(dToR*3), 0, 0);
    matrix_Z.y = point4(-sin(dToR*3), cos(dToR*3), 0, 0);
    matrix_Z.z = point4(0, 0, 1, 0);
    matrix_Z.w = point4(0, 0, 0, 1);
    
    //Initilizaing the eyeP and centerP
    eyeP = point4(0, 7, 7, 1.0);
    centerP = point4(0, 0, 0, 1.0);
    
    //Conventions
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
    
    //Locating the reference to the uniform variables inside the shader file
    model_location = glGetUniformLocation(program, "model_view");
    project_location = glGetUniformLocation(program, "projection");
    ambinent_P = glGetUniformLocation(program, "AmbientProduct");
    diffuse_P = glGetUniformLocation(program, "DiffuseProduct");
    specular_P = glGetUniformLocation(program, "SpecularProduct");
    light_Position = glGetUniformLocation(program, "LightPosition");
    shiness = glGetUniformLocation(program, "shininess");
    atten_Constant = glGetUniformLocation(program, "attenuation_constant");
    atten_linear = glGetUniformLocation(program, "attenuation_linear");
    atten_quatric = glGetUniformLocation(program, "attenuation_quadratic");
    isS = glGetUniformLocation(program, "isShadow");
    blackC = glGetUniformLocation(program, "black");
    
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    
    
}//end of init


void display(void)
{
    
    int temp = 0;
    
    glClearDepth(1);
    
    //glClearColor (1,0,0,1.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    glUniformMatrix4fv(project_location, 1, GL_FALSE, (GLfloat *) &tr2_matrix);
    glUniform4fv(light_Position, 1, (GLfloat *) &light_P);
    glUniform4fv(blackC, 1, (GLfloat *) &black_Color);
    
    //Calculating the variables for the red ball
    amb_P = point4(0.2, 0, 0, 1);
    diffu_P = point4(1, 0, 0, 1);
    spec_P = point4(1, 1, 1, 1);
    shin =  10;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    
    
    //Drawing the shadow for the red ball
    glUniform1i(isS, 1);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    temp += 1128;
    
    
    //The green ball
    amb_P = point4(0, 0.2, 0, 1);
    diffu_P = point4(0, 1, 0, 1);
    spec_P = point4(1, 1, 1, 1);
    shin =  10;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    glUniform1i(isS, 0);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    //Drawing the shadow for the green ball
    glUniform1i(isS, 1);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    temp += 1128;
    
    //The blue ball
    amb_P = point4(0, 0, 0.2, 1);
    diffu_P = point4(0, 0, 1, 1);
    spec_P = point4(1, 1, 1, 1);
    shin =  10;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    glUniform1i(isS, 0);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    
    //Draw the shadow for the blue ball
    glUniform1i(isS, 1);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    temp += 1128;
    
    //The yellow ball
    amb_P = point4(0.2, 0.2, 0, 1);
    diffu_P = point4(1, 1, 0, 1);
    spec_P = point4(1, 1, 1, 1);
    shin =  10;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    glUniform1i(isS, 0);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    //Draw the shadow of yellow ball
    glUniform1i(isS, 1);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    temp += 1128;
    
    //the brown ball
    amb_P = point4(0.2, 0.1, 0, 1);
    diffu_P = point4(1, 0.5, 0, 1);
    spec_P = point4(1, 1, 1, 1);
    shin =  10;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    glUniform1i(isS, 0);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    //The shadow of brown ball
    glUniform1i(isS, 1);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    temp += 1128;
    
    //The light source, white ball
    amb_P = point4(0.04, 0.04, 0.04, 1);
    diffu_P = point4(1,1, 1, 1);
    spec_P = point4(1, 1, 1, 1);
    shin = 0;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    glUniform1i(isS, 0);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 1128);
    
    
    temp += 1128;
    
    //Our plane
    amb_P = point4(0, 0.6, 0, 1);
    diffu_P = point4(0, 1, 0, 1);
    spec_P = point4(1, 1, 1, 1);
    shin =  10;
    
    glUniform4fv(ambinent_P, 1, (GLfloat *) &amb_P);
    glUniform4fv(diffuse_P, 1, (GLfloat *) &diffu_P);
    glUniform4fv(specular_P, 1, (GLfloat *) &spec_P);
    glUniform1fv(shiness, 1, (GLfloat *) &shin);
    glUniform1fv(atten_Constant, 1, (GLfloat *) &att_C);
    glUniform1fv(atten_linear, 1, (GLfloat *) &att_L);
    glUniform1fv(atten_quatric, 1, (GLfloat *) &att_Q);
    
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, temp, 36);
    
    
    //printf("%d\n",temp);
    
    glutSwapBuffers();
}//end of display



void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'q' || key == 'Q')
    {
        exit(0);
    }
    
    
    if (key == 'g' || key == 'G')//start to rotate the balls
    {
        enable = 1;
    }
    
    
    //Controlling the light source
    if (key == 'a' || key == 'A')
    {
        light_P = point4(light_P.x-0.05, light_P.y, light_P.z, 1.0);
        //Move the light ball
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f+where_light] = point4(vertices[f+where_light].x-0.05, vertices[f+where_light].y, vertices[f+where_light].z, 1.0);
            //normals[f+where_light] = point4(normals[f+where_light].x-0.03, normals[f+where_light].y, normals[f+where_light].z, 0.0);
            normals[f+where_light] = normalizeVec4(point4(normals[f+where_light].x-0.05, normals[f+where_light].y, normals[f+where_light].z, 0.0));
        }
    }
    else if (key == 'd' || key == 'D')
    {
        light_P = point4(light_P.x+0.05, light_P.y, light_P.z, 1.0);
        //Move the light ball
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f+where_light] = point4(vertices[f+where_light].x+0.05, vertices[f+where_light].y, vertices[f+where_light].z, 1.0);
            //normals[f+where_light] = point4(normals[f+where_light].x+0.03, normals[f+where_light].y, normals[f+where_light].z, 0.0);
            normals[f+where_light] = normalizeVec4(point4(normals[f+where_light].x+0.05, normals[f+where_light].y, normals[f+where_light].z, 0.0));
        }
    }
    else if (key == 'w' || key == 'W')
    {
        light_P = point4(light_P.x, light_P.y+0.05, light_P.z, 1.0);
        //Move the light ball
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f+where_light] = point4(vertices[f+where_light].x, vertices[f+where_light].y+0.05, vertices[f+where_light].z, 1.0);
            //normals[f+where_light] = point4(normals[f+where_light].x, normals[f+where_light].y+0.03, normals[f+where_light].z, 0.0);
            normals[f+where_light] = normalizeVec4(point4(normals[f+where_light].x, normals[f+where_light].y+0.05, normals[f+where_light].z, 0.0));
        }
    }
    else if (key == 's' || key == 'S')
    {
        light_P = point4(light_P.x, light_P.y-0.05, light_P.z, 1.0);
        //Move the light ball
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f+where_light] = point4(vertices[f+where_light].x, vertices[f+where_light].y-0.05, vertices[f+where_light].z, 1.0);
            //normals[f+where_light] = point4(normals[f+where_light].x, normals[f+where_light].y-0.03, normals[f+where_light].z, 0.0);
            normals[f+where_light] = normalizeVec4(point4(normals[f+where_light].x, normals[f+where_light].y-0.05, normals[f+where_light].z, 0.0));
        }
    }
    else if (key == 'z' || key == 'Z')//moving in positive direction of z
    {
        light_P = point4(light_P.x, light_P.y, light_P.z+0.05, 1.0);
        //Move the light ball
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f+where_light] = point4(vertices[f+where_light].x, vertices[f+where_light].y, vertices[f+where_light].z+0.05, 1.0);
            //normals[f+where_light] = point4(normals[f+where_light].x, normals[f+where_light].y-0.03, normals[f+where_light].z, 0.0);
            normals[f+where_light] = normalizeVec4(point4(normals[f+where_light].x, normals[f+where_light].y, normals[f+where_light].z+0.05, 0.0));
        }
    }
    else if (key == 'x' || key == 'X')//moving in the negative direction of z
    {
        light_P = point4(light_P.x, light_P.y, light_P.z-0.05, 1.0);
        //Move the light ball
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f+where_light] = point4(vertices[f+where_light].x, vertices[f+where_light].y, vertices[f+where_light].z-0.05, 1.0);
            //normals[f+where_light] = point4(normals[f+where_light].x, normals[f+where_light].y-0.03, normals[f+where_light].z, 0.0);
            normals[f+where_light] = normalizeVec4(point4(normals[f+where_light].x, normals[f+where_light].y, normals[f+where_light].z-0.05, 0.0));
        }
    }
    
    
    
    
    //controlling the camera
    if (key == 'i' || key == 'I')
    {
        eyeP = pointMulti44Times41(matrix_X, eyeP);
    }
    else if (key == 'k' || key == 'K')
    {
        eyeP = pointMulti44Times41(matrix_XN, eyeP);
    }
    else if (key == 'j' || key == 'J')
    {
        eyeP = pointMulti44Times41(matrix_YN, eyeP);
    }
    else if (key == 'l' || key == 'L')
    {
        eyeP = pointMulti44Times41(matrix_Y, eyeP);
    }
    
    
    //Moving the camera closer or farer away
    if (key == 't' || key == 'T')//moving closer
    {
        vec4 futaba = SubV(origin, eyeP);
        eyeP = AddiV(eyeP, ScalarMulti(0.05, futaba));
    }
    else if (key == 'y' || key == 'Y')//moving away
    {
        vec4 futaba = SubV(eyeP, origin);
        eyeP = AddiV(eyeP, ScalarMulti(0.05, futaba));
    }
    
    
    
    
    
    tr_matrix = look_at(eyeP.x, eyeP.y, eyeP.z, centerP.x, centerP.y, centerP.z, 0.0, 1.0, 0.0);
    tr2_matrix = frustum(-0.3, 0.3, -0.3, 0.3, -0.5,-100);
    
    //Repush the new modified array back into the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    
     glutPostRedisplay();
}//end of keyboard


void mouse(int button, int state, int x, int y)
{
    
    
    
    
}//end of mouse function





void idle (void)
{
    
    
    if (enable == 1)
    {
        //Make the balls rotating around the red ball
        //First the green ball
        
        int joker = 1128;
        
        for (int f = joker; f < (joker+1128); f ++)
        {
            vec4 futaba = point4(vertices[f].x, vertices[f].y-0.5, vertices[f].z, 1.0);
            futaba = pointMulti44Times41(matrix_green, futaba);
            vertices[f] = point4(futaba.x, futaba.y+0.5, futaba.z, 1.0);
            //normals[f] = normalizeVec4(SubV(point4(futaba.x, futaba.y+0.5, futaba.z, 1.0), pointMulti44Times41(matrix_green, center_Green)));
            normals[f] = normalizeVec4(pointMulti44Times41(matrix_green, normals[f]));
        }
        
        joker += 1128;
        
        //Next is our blue ball
        
        for (int f = joker; f < (joker+1128); f ++)
        {
            vec4 futaba = point4(vertices[f].x, vertices[f].y-0.5, vertices[f].z, 1.0);
            futaba = pointMulti44Times41(matrix_blue, futaba);
            vertices[f] = point4(futaba.x, futaba.y+0.5, futaba.z, 1.0);
            //normals[f] = normalizeVec4(SubV(point4(futaba.x, futaba.y+0.5, futaba.z, 1.0),pointMulti44Times41(matrix_blue, center_Blue)));
            normals[f] = normalizeVec4(pointMulti44Times41(matrix_blue, normals[f]));
        }
        
        joker += 1128;
        
        
        //Yellow ball
        
        for (int f = joker; f < (joker+1128); f ++)
        {
            vec4 futaba = point4(vertices[f].x, vertices[f].y-0.5, vertices[f].z, 1.0);
            futaba = pointMulti44Times41(matrix_yellow, futaba);
            vertices[f] = point4(futaba.x, futaba.y+0.5, futaba.z, 1.0);
            //normals[f] = normalizeVec4(SubV(point4(futaba.x, futaba.y+0.5, futaba.z, 1.0), pointMulti44Times41(matrix_yellow, center_Yellow)));
            normals[f] = normalizeVec4(pointMulti44Times41(matrix_yellow, normals[f]));
        }
        
        joker += 1128;
        
        //Finally the brown ball
        for (int f = joker; f < (joker+1128); f ++)
        {
            vec4 futaba = point4(vertices[f].x, vertices[f].y-0.5, vertices[f].z, 1.0);
            futaba = pointMulti44Times41(matrix_brown, futaba);
            vertices[f] = point4(futaba.x, futaba.y+0.5, futaba.z, 1.0);
            //normals[f] = normalizeVec4(SubV(point4(futaba.x, futaba.y+0.5, futaba.z, 1.0), pointMulti44Times41(matrix_brown, center_Brown)));
            normals[f] = normalizeVec4(pointMulti44Times41(matrix_brown, normals[f]));
        }
        
        joker += 1128;
        
        //Done with the rotation
    }
    
    
    tr_matrix = look_at(eyeP.x, eyeP.y, eyeP.z, centerP.x, centerP.y, centerP.z, 0.0, 1.0, 0.0);
    tr2_matrix = frustum(-0.3, 0.3, -0.3, 0.3, -0.5,-100);
    
    
    //Repush the new modified array back into the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    
    glutPostRedisplay();
}//end of idle function


int main(int argc, char **argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project3 ZhishengXu");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}//end of main


