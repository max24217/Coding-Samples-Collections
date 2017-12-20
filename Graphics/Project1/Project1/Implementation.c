//
//  The actual implementation for all the helper functions in Library.h
//
//  Since we are using these helper functions to use in our projects, W will always be zero if the result is a vector
//  while 1 if the result if a point.
//
//  Created by XuZhisheng on 9/21/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//

#include "Library.h"
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


vec3 point3(GLfloat a, GLfloat b, GLfloat c)
{
    vec3 akb48;
    akb48.x = a;
    akb48.y = b;
    akb48.z = c;
    
    return akb48;
}//end of point3


//Used to print out a vector4. Three digits after decmial point.
void printVector(vec4 a)
{
    printf("(%.3f, %.3f, %.3f, %.3f)",a.x, a.y, a.z, a.w);
    
}//end of printVector



//Perform cross product on two vectors and return result vector4
vec4 crossProduct(vec4 a,vec4 b)
{
    vec4 out;
    
    out.x = a.y * b.z - a.z * b.y;
    out.y = -(a.x * b.z - a.z * b.x);
    out.z = a.x * b.y - a.y * b.x;
    out.w = 0;
    
    return out;
    
}//end of crossProduct


//Perform dot product on two vectors and return a GLfloat as the result
GLfloat dotProduct(vec4 a,vec4 b)
{
    GLfloat out = a.x*b.x + a.y*b.y + a.z*b.z;
    return out;
}//end of dotProduct



//Scalar multipulication on a single vector4. Return the result vector
vec4 ScalarMulti(GLfloat a, vec4 b)
{
    b.x = b.x * a;
    b.y = b.y * a;
    b.z = b.z * a;
    b.w = 0;
    return b;
}//end of ScalarMulti



//Addition bewtween two vectors and return the result vector
vec4 AddiV(vec4 a,vec4 b)
{
    vec4 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = 0;
    
    return c;
}//end of Addiv


//Substraction between two vectors and return the result vector
vec4 SubV(vec4 a, vec4 b)
{
    vec4 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = 0;
    
    return c;
}//end of SubV


//Initinalizing a vec4
vec4 point4(GLfloat a,GLfloat b, GLfloat c, GLfloat d)
{
    vec4 snh48;
    snh48.x = a;
    snh48.y = b;
    snh48.z = c;
    snh48.w = d;
    
    return snh48;
}//end of point4

//scale a vec4
vec4 scaleVec4(GLfloat xS, GLfloat yS, GLfloat zS, GLfloat wS, vec4 into)
{
    vec4 out;
    out.x = into.x * xS;
    out.y = into.y * yS;
    out.z = into.z * zS;
    out.w = into.w * wS;
    
    return out;
}//end of scaleVec4


//Used to do multiplication on 44 to 41 matrix, result will be a point
vec4 pointMulti44Times41(mat4 a, vec4 b)
{
    vec4 result;
    result.x = a.x.x*b.x + a.y.x*b.y + a.z.x*b.z +a.w.x*b.w;
    result.y = a.x.y*b.x + a.y.y*b.y + a.z.y*b.z +a.w.y*b.w;
    result.z = a.x.z*b.x + a.y.z*b.y + a.z.z*b.z +a.w.z*b.w;
    result.w = 1;
    
    return result;
}//end of pointMulti44Times41



//Print out a 4X4 matrix, colunm first
void printMatrix(mat4 a)
{
    printf("%.3f   %.3f   %.3f   %.3f\n",a.x.x,a.y.x,a.z.x,a.w.x);
    printf("\n");
    printf("%.3f   %.3f   %.3f   %.3f\n",a.x.y,a.y.y,a.z.y,a.w.y);
    printf("\n");
    printf("%.3f   %.3f   %.3f   %.3f\n",a.x.z,a.y.z,a.z.z,a.w.z);
    printf("\n");
    printf("%.3f   %.3f   %.3f   %.3f\n",a.x.w,a.y.w,a.z.w,a.w.w);
    printf("\n");
    
}//end of printMatrix



//Scalar multipulication of 4X4 matric and return the result matrix
mat4 ScalMultiMat4(GLfloat a,mat4 b)
{
    b.x.x = b.x.x*a;
    b.x.y = b.x.y*a;
    b.x.z = b.x.z*a;
    b.x.w = b.x.w*a;
    
    b.y.x = b.y.x*a;
    b.y.y = b.y.y*a;
    b.y.z = b.y.z*a;
    b.y.w = b.y.w*a;
    
    b.z.x = b.z.x*a;
    b.z.y = b.z.y*a;
    b.z.z = b.z.z*a;
    b.z.w = b.z.w*a;
    
    b.w.x = b.w.x*a;
    b.w.y = b.w.y*a;
    b.w.z = b.w.z*a;
    b.w.w = b.w.w*a;
    
    return b;
}//end of ScalMultiMat4



//The addition of two matrics and return the result matrix
mat4 AddMat4(mat4 a,mat4 b)
{
    b.x.x = b.x.x + a.x.x;
    b.x.y = b.x.y + a.x.y;
    b.x.z = b.x.z + a.x.z;
    b.x.w = b.x.w + a.x.w;
    
    b.y.x = b.y.x + a.y.x;
    b.y.y = b.y.y + a.y.y;
    b.y.z = b.y.z + a.y.z;
    b.y.w = b.y.w + a.y.w;
    
    b.z.x = b.z.x + a.z.x;
    b.z.y = b.z.y + a.z.y;
    b.z.z = b.z.z + a.z.z;
    b.z.w = b.z.w + a.z.w;
    
    b.w.x = b.w.x + a.w.x;
    b.w.y = b.w.y + a.w.y;
    b.w.z = b.w.z + a.w.z;
    b.w.w = b.w.w + a.w.w;
    
    return b;
}//end of AddMat4



//The substraction between two matrix and return the result matrix
mat4 SubMat4(mat4 a,mat4 b)
{
    b.x.x = a.x.x - b.x.x;
    b.x.y = a.x.y - b.x.y;
    b.x.z = a.x.z - b.x.z;
    b.x.w = a.x.w - b.x.w;
    
    b.y.x = a.y.x - b.y.x;
    b.y.y = a.y.y - b.y.y;
    b.y.z = a.y.z - b.y.z;
    b.y.w = a.y.w - b.y.w;
    
    b.z.x = a.z.x - b.z.x;
    b.z.y = a.z.y - b.z.y;
    b.z.z = a.z.z - b.z.z;
    b.z.w = a.z.w - b.z.w;
    
    b.w.x = a.w.x - b.w.x;
    b.w.y = a.w.y - b.w.y;
    b.w.z = a.w.z - b.w.z;
    b.w.w = a.w.w - b.w.w;
    
    
    return b;
}//end of SubMat4



//Multipulication between two matrix and return result matrix
mat4 MultiMat4(mat4 a, mat4 b)
{
    mat4 akb48;
    akb48.x.x = a.x.x*b.x.x + a.y.x*b.x.y + a.z.x*b.x.z + a.w.x*b.x.w;
    akb48.y.x = a.x.x*b.y.x + a.y.x*b.y.y + a.z.x*b.y.z + a.w.x*b.y.w;
    akb48.z.x = a.x.x*b.z.x + a.y.x*b.z.y + a.z.x*b.z.z + a.w.x*b.z.w;
    akb48.w.x = a.x.x*b.w.x + a.y.x*b.w.y + a.z.x*b.w.z + a.w.x*b.w.w;
    
    akb48.x.y = a.x.y*b.x.x + a.y.y*b.x.y + a.z.y*b.x.z + a.w.y*b.x.w;
    akb48.y.y = a.x.y*b.y.x + a.y.y*b.y.y + a.z.y*b.y.z + a.w.y*b.y.w;
    akb48.z.y = a.x.y*b.z.x + a.y.y*b.z.y + a.z.y*b.z.z + a.w.y*b.z.w;
    akb48.w.y = a.x.y*b.w.x + a.y.y*b.w.y + a.z.y*b.w.z + a.w.y*b.w.w;
    
    akb48.x.z = a.x.z*b.x.x + a.y.z*b.x.y + a.z.z*b.x.z + a.w.z*b.x.w;
    akb48.y.z = a.x.z*b.y.x + a.y.z*b.y.y + a.z.z*b.y.z + a.w.z*b.y.w;
    akb48.z.z = a.x.z*b.z.x + a.y.z*b.z.y + a.z.z*b.z.z + a.w.z*b.z.w;
    akb48.w.z = a.x.z*b.w.x + a.y.z*b.w.y + a.z.z*b.w.z + a.w.z*b.w.w;
    
    akb48.x.w = a.x.w*b.x.x + a.y.w*b.x.y + a.z.w*b.x.z + a.w.w*b.x.w;
    akb48.y.w = a.x.w*b.y.x + a.y.w*b.y.y + a.z.w*b.y.z + a.w.w*b.y.w;
    akb48.z.w = a.x.w*b.z.x + a.y.w*b.z.y + a.z.w*b.z.z + a.w.w*b.z.w;
    akb48.w.w = a.x.w*b.w.x + a.y.w*b.w.y + a.z.w*b.w.z + a.w.w*b.w.w;
    
    return akb48;
}//end of MultiMat4


//Transpose a single matrix and return the result matrix
mat4 TransposeMat4(mat4 a)
{
    mat4 ak;
    ak.x.x = a.x.x;
    ak.x.y = a.y.x;
    ak.x.z = a.z.x;
    ak.x.w = a.w.x;
    
    ak.y.x = a.x.y;
    ak.y.y = a.y.y;
    ak.y.z = a.z.y;
    ak.y.w = a.w.y;
    
    ak.z.x = a.x.z;
    ak.z.y = a.y.z;
    ak.z.z = a.z.z;
    ak.z.w = a.w.z;
    
    ak.w.x = a.x.w;
    ak.w.y = a.y.w;
    ak.w.z = a.z.w;
    ak.w.w = a.w.w;
    
    return ak;
}//end of TransposeMat4



//Inverse a matrix and return the result matrix
mat4 InverseMat4(mat4 a)
{
    //Calculating the matrix of minors
    mat4 ak;
    ak.x.x = a.y.y*a.z.z*a.w.w + a.z.y*a.w.z*a.y.w + a.w.y*a.y.z*a.z.w - a.y.w*a.z.z*a.w.y - a.z.w*a.w.z*a.y.y - a.w.w*a.y.z*a.z.y;
    ak.x.y = a.y.x*a.z.z*a.w.w + a.z.x*a.w.z*a.y.w + a.w.x*a.y.z*a.z.w - a.y.w*a.z.z*a.w.x - a.z.w*a.w.z*a.y.x - a.w.w*a.y.z*a.z.x;
    ak.x.z = a.y.x*a.z.y*a.w.w + a.z.x*a.w.y*a.y.w + a.w.x*a.y.y*a.z.w - a.y.w*a.z.y*a.w.x - a.z.w*a.w.y*a.y.x - a.w.w*a.y.y*a.z.x;
    ak.x.w = a.y.x*a.z.y*a.w.z + a.z.x*a.w.y*a.y.z + a.w.x*a.y.y*a.z.z - a.y.z*a.z.y*a.w.x - a.z.z*a.w.y*a.y.x - a.w.z*a.y.y*a.z.x;
    
    ak.y.x = a.x.y*a.z.z*a.w.w + a.z.y*a.w.z*a.x.w + a.w.y*a.x.z*a.z.w - a.x.w*a.z.z*a.w.y - a.z.w*a.w.z*a.x.y - a.w.w*a.x.z*a.z.y;
    ak.y.y = a.x.x*a.z.z*a.w.w + a.z.x*a.w.z*a.x.w + a.w.x*a.x.z*a.z.w - a.x.w*a.z.z*a.w.x - a.z.w*a.w.z*a.x.x - a.w.w*a.x.z*a.z.x;
    ak.y.z = a.x.x*a.z.y*a.w.w + a.z.x*a.w.y*a.x.w + a.w.x*a.x.y*a.z.w - a.x.w*a.z.y*a.w.x - a.z.w*a.w.y*a.x.x - a.w.w*a.x.y*a.z.x;
    ak.y.w = a.x.x*a.z.y*a.w.z + a.z.x*a.w.y*a.x.z + a.w.x*a.x.y*a.z.z - a.x.z*a.z.y*a.w.x - a.z.z*a.w.y*a.x.x - a.w.z*a.x.y*a.z.x;
    
    ak.z.x = a.x.y*a.y.z*a.w.w + a.y.y*a.w.z*a.x.w + a.w.y*a.x.z*a.y.w - a.x.w*a.y.z*a.w.y - a.y.w*a.w.z*a.x.y - a.w.w*a.x.z*a.y.y;
    ak.z.y = a.x.x*a.y.z*a.w.w + a.y.x*a.w.z*a.x.w + a.w.x*a.x.z*a.y.w - a.x.w*a.y.z*a.w.x - a.y.w*a.w.z*a.x.x - a.w.w*a.x.z*a.y.x;
    ak.z.z = a.x.x*a.y.y*a.w.w + a.y.x*a.w.y*a.x.w + a.w.x*a.x.y*a.y.w - a.x.w*a.y.y*a.w.x - a.y.w*a.w.y*a.x.x - a.w.w*a.x.y*a.y.x;
    ak.z.w = a.x.x*a.y.y*a.w.z + a.y.x*a.w.y*a.x.z + a.w.x*a.x.y*a.y.z - a.x.z*a.y.y*a.w.x - a.y.z*a.w.y*a.x.x - a.w.z*a.x.y*a.y.x;
    
    ak.w.x = a.x.y*a.y.z*a.z.w + a.y.y*a.z.z*a.x.w + a.z.y*a.x.z*a.y.w - a.x.w*a.y.z*a.z.y - a.y.w*a.z.z*a.x.y - a.z.w*a.x.z*a.y.y;
    ak.w.y = a.x.x*a.y.z*a.z.w + a.y.x*a.z.z*a.x.w + a.z.x*a.x.z*a.y.w - a.x.w*a.y.z*a.z.x - a.y.w*a.z.z*a.x.x - a.z.w*a.x.z*a.y.x;
    ak.w.z = a.x.x*a.y.y*a.z.w + a.y.x*a.z.y*a.x.w + a.z.x*a.x.y*a.y.w - a.x.w*a.y.y*a.z.x - a.y.w*a.z.y*a.x.x - a.z.w*a.x.y*a.y.x;
    ak.w.w = a.x.x*a.y.y*a.z.z + a.y.x*a.z.y*a.x.z + a.z.x*a.x.y*a.y.z - a.x.z*a.y.y*a.z.x - a.y.z*a.z.y*a.x.x - a.z.z*a.x.y*a.y.x;
    
    //The dominant of the original matrix
    GLfloat dominant = a.x.x*ak.x.x - a.x.y*ak.x.y + a.x.z*ak.x.z - a.x.w*ak.x.w;
    
    
    //Doing cofactor
    ak.x.y = -ak.x.y;
    ak.x.w = -ak.x.w;
    ak.y.x = -ak.y.x;
    ak.y.z = -ak.y.z;
    ak.z.y = -ak.z.y;
    ak.z.w = -ak.z.w;
    ak.w.x = -ak.w.x;
    ak.w.z = -ak.w.z;
    
    //Transpose the cofactor
    ak = TransposeMat4(ak);
    //Form the reverse matrix of the input matrix
    ak = ScalMultiMat4(1/dominant, ak);
    
    return ak;
}//end of InverseMat4




mat4 rotat_Z(GLfloat theta)
{
    
    mat4 result;
    float s = (M_PI/180.0)*theta;
    
    result.x.x = cos(s);
    result.y.x = -sin(s);
    result.z.x = 0;
    result.w.x = 0;
    result.x.y = sin(s);
    result.y.y = cos(s);
    result.z.y = 0;
    result.w.y = 0;
    result.x.z = 0;
    result.y.z = 0;
    result.z.z = 1;
    result.w.z = 0;
    result.x.w = 0;
    result.y.w = 0;
    result.z.w = 0;
    result.w.w = 1;
    
    return result;
}//end of rotat_Z



mat4 rotat_Y(GLfloat theta)
{
    mat4 result;
    float s = (M_PI/180.0)*theta;
    
   
    
    result.x.x = cos(s);
    result.x.y = 0;
    result.x.z = -sin(s);
    result.x.w = 0;
    
    result.y.x = 0;
    result.y.y = 1;
    result.y.z = 0;
    result.y.w = 0;
    
    result.z.x = sin(s);
    result.z.y = 0;
    result.z.z = -cos(s);
    result.z.z = cos(s);
    result.z.w = 0;
    
    result.w.x = 0;
    result.w.y = 0;
    result.w.z = 0;
    result.w.w = 1;
    
    return result;
}//end of rotat_Y



mat4 rotat_X(GLfloat theta)
{
    mat4 result;
    float s = (M_PI/180.0)*theta;
    
    
    
    
    result.x.x = 1;
    result.x.y = 0;
    result.x.z = 0;
    result.x.w = 0;
    
    result.y.x = 0;
    result.y.y = cos(s);
    result.y.z = sin(s);
    result.y.w = 0;
    
    result.z.x = 0;
    result.z.y = -sin(s);
    result.z.z = cos(s);
    result.z.w = 0;
    
    result.w.x = 0;
    result.w.y = 0;
    result.w.z = 0;
    result.w.w = 1;
    
    return result;
}//end of rotat_X



//Calculate distance between two points
GLfloat distance(vec4 a, vec4 b)
{
    GLfloat first = (a.x-b.x) * (a.x-b.x);
    GLfloat second = (a.y-b.y) * (a.y-b.y);
    GLfloat third = (a.z-b.z) * (a.z-b.z);
    
    
    return sqrt(first + second + third);
}//end of distance



vec4 screenToWorldVec4(GLfloat windosSize, vec4 a)
{
    vec4 out;
    out.x = a.x - windosSize/2;
    out.y = a.y - windosSize/2;
    out.z = a.z;
    out.w = 0.0;
    
    return out;
}//end of screenToWorldVec4



vec4 normalizeVec4(vec4 a)
{
    GLfloat mag = distance(a, point4(0.0, 0.0, 0.0, 1.0));
    vec4 out = point4(a.x/mag, a.y/mag, a.z/mag, 0.0);

    return out;
}//end of normalizeVec4


mat4 identityMatrix()
{
    mat4 out;
    out.x = point4(1.0, 0.0, 0.0, 0.0);
    out.y = point4(0.0, 1.0, 0.0, 0.0);
    out.z = point4(0.0, 0.0, 1.0, 0.0);
    out.w = point4(0.0, 0.0, 0.0, 1.0);
    
    return out;
    
}//end of identityMatrix
