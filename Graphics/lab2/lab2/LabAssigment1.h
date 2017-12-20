//
//  LabAssigment1.h
//  lab2 (Assignment 1)
//
//  Created by XuZhisheng on 9/21/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//

#include <stdio.h>
#include <Math.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__


//struct for vector4
typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4 ;

//struct for a 4X4 matrix, colunm first
typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;


//Predeclare the header for utiliy functions
void printVector(vec4 a);

vec4 crossProduct(vec4 a,vec4 b);

GLfloat dotProduct(vec4 a,vec4 b);

vec4 ScalarMulti(GLfloat a,vec4 b);

vec4 AddiV(vec4 a, vec4 b);

vec4 SubV(vec4 a, vec4 b);

void printMatrix(mat4 a);

mat4 ScalMultiMat4(GLfloat a, mat4 b);

mat4 AddMat4(mat4 a,mat4 b);

mat4 SubMat4(mat4 a,mat4 b);

mat4 MultiMat4(mat4 a, mat4 b);

mat4 InverseMat4(mat4 a);

mat4 TransposeMat4(mat4 a);






