//
//  LabAssigmentMain.c
//  lab2 (Assignment 1)
//
//  Created by XuZhisheng on 9/21/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//

#include "LabAssigment1.h"
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


int main(int argc, const char* argv[])
{
    
    vec4 a;
    a.x = -4;
    a.y = -1;
    a.z = -4;
    a.w = 0;
    
    //testing printing method for vector
    printf("Testing printing method for vector\n");
    printVector(a);
    printf("\n");
    printf("\n");
    
    vec4 b;
    b.x = 1;
    b.y = 5;
    b.z = 7;
    b.w = 0;
    
    //testing cross product method
    vec4 c = crossProduct(a, b);
    
    
    //To see if the result is correct
    printf("Testing cross product method for two vectors\n");
    printVector(a);
    printf(" X ");
    printVector(b);
    printf("\n");
    printf("The result is:\n");
    printVector(c);
    printf("\n");
    printf("\n");
    
    //Testing dot product
    printf("Testing dot product method\n");
    printVector(a);
    printf(" dot product with ");
    printVector(b);
    printf("\n");
    printf("The result is:\n");
    GLfloat d = dotProduct(a,b);
    printf("%.2f",d);
    printf("\n");
    printf("\n");
    
    
    //Testing scalar multi for vector
    printf("Testing scalar multi for vector\n");
    printVector(a);
    printf(" X 4.\nThe result is:\n");
    c = ScalarMulti(4.00, a);
    printVector(c);
    printf("\n");
    printf("\n");
    
    
    //Testing Addition of two vectors
    printf("Testing the addition method for two vectors\n");
    printVector(a);
    printf(" + ");
    printVector(b);
    printf("\n");
    printf("The result is:\n");
    c = AddiV(a, b);
    printVector(c);
    printf("\n");
    printf("\n");
    
    //Testing Subtraction of two vectors
    printf("Testing the subtraction method for two vectors\n");
    printVector(a);
    printf(" - ");
    printVector(b);
    printf("\n");
    printf("The result is:\n");
    c = SubV(a, b);
    printVector(c);
    printf("\n");
    printf("\n");
    
    
    //Testing the print out function for 4X4 matrics
    printf("Testing the print out function for 4 X 4 matrics\n");
    
    //Initializing the matrics and its vectors
    //This matrix comes from Dr. Tan's slide on inverse of matrix
    mat4 k;
    
    vec4 first;
    first.x = -4;
    first.y = -1;
    first.z = -4;
    first.w = -1;
    
    vec4 second;
    second.x = 0;
    second.y = 2;
    second.z = 4;
    second.w = -9;
    
    vec4 third;
    third.x = 0;
    third.y = 0;
    third.z = 4;
    third.w = -1;
    
    vec4 fourth;
    fourth.x = 0;
    fourth.y = 0;
    fourth.z = 0;
    fourth.w = 1;
    
    k.x = first;
    k.y = second;
    k.z = third;
    k.w = fourth;
    
    //Start to print out the matrix
    printMatrix(k);
    
    printf("\n");
    printf("\n");
    
    
    //Testing the scalar multi of matrix
    printf("Testing the scalar multi method for matrix\n");
    printMatrix(k);
    printf(" X 2 ");
    printf("The result is:\n");
    mat4 mk = ScalMultiMat4(2.00, k);
    printMatrix(mk);
    printf("\n");
    printf("\n");
    
    
    //Testing AddMat4 function for matrix
    printf("Testing the scalar multi method for matrix\n");
    printMatrix(k);
    printf(" is added to itself. ");
    printf("The result is:\n");
    mk = AddMat4(k,k);
    printMatrix(mk);
    printf("\n");
    printf("\n");
    
    
    //Testing SubMat4 function for matrix
    printf("Testing the SubMat4 method for matrix\n");
    printMatrix(k);
    printf(" is subtracted by itself. ");
    printf("The result is:\n");
    mk = SubMat4(k,k);
    printMatrix(mk);
    printf("\n");
    printf("\n");
    
    
    //Testing MultiMat4 function for matrix
    printf("Testing the MultiMat4 function for matrix\n");
    printMatrix(k);
    printf(" multi to itself. The result is: \n");
    mk = MultiMat4(k, k);
    printMatrix(mk);
    printf("\n");
    printf("\n");

    
    //Testing the transpose function for matrix
    printf("Testing the transpose function for matrix\n");
    printMatrix(k);
    printf(" is transposed. The result is: \n");
    mk = TransposeMat4(k);
    printMatrix(mk);
    printf("\n");
    printf("\n");
    
    
    //Testing the inverse function for matrix
    printf("Testing the inverse function for matrix\n");
    printMatrix(k);
    printf(" is inversed. The result is: \n");
    mk = InverseMat4(k);
    printMatrix(mk);
    printf("\n");
    printf("\n");
    
    printf("Let's check the inverse matrix. Let\n");
    printMatrix(k);
    printf("\n times with\n");
    printMatrix(mk);
    printf("\nThe result is:\n");
    mk = MultiMat4(k, mk);
    printMatrix(mk);
    printf("\nThe result is an identity matrix which shows the inverse function is correct");
    //Noted that the correct answer on the powerpoint is wrong, x.w should be -3/2 instead of 3/2.
    //Dr. Tan mentioned this during lecture and I used online tool to confirme that. If x.w is 3/2, the final result matrix will not be an identity matrix
    printf("\n");
    printf("\n");

    
    return 0;
}//end of main
