//
//
// The library for helper functions
//
//  Created by XuZhisheng on 9/21/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//

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
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


//Struct for vector3
typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3;


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
vec3 point3(GLfloat a, GLfloat b, GLfloat c);

void printVector(vec4 a);

vec4 crossProduct(vec4 a,vec4 b);

GLfloat dotProduct(vec4 a,vec4 b);

vec4 ScalarMulti(GLfloat a,vec4 b);

vec4 AddiV(vec4 a, vec4 b);

vec4 SubV(vec4 a, vec4 b);

vec4 point4(GLfloat a, GLfloat b, GLfloat c, GLfloat d);

vec4 scaleVec4(GLfloat xS, GLfloat yS, GLfloat zS, GLfloat wS, vec4 into);

vec4 pointMulti44Times41(mat4 a, vec4 b);

void printMatrix(mat4 a);

mat4 ScalMultiMat4(GLfloat a, mat4 b);

mat4 AddMat4(mat4 a,mat4 b);

mat4 SubMat4(mat4 a,mat4 b);

mat4 MultiMat4(mat4 a, mat4 b);

mat4 InverseMat4(mat4 a);

mat4 TransposeMat4(mat4 a);

mat4 rotat_Z(GLfloat theata);

mat4 rotat_Y(GLfloat theata);

mat4 rotat_X(GLfloat theata);

mat4 identityMatrix();

mat4 translation(GLfloat tX, GLfloat tY, GLfloat tZ);

GLfloat distance(vec4 a, vec4 b);

vec4 screenToWorldVec4(GLfloat windowsSize, vec4 a);

vec4 normalizeVec4(vec4 a);

mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,GLfloat near, GLfloat far);

mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez,GLfloat atx, GLfloat aty, GLfloat atz, GLfloat upx, GLfloat upy, GLfloat upz);

mat4 identity();
