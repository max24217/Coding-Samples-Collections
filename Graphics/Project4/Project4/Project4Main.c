//
//  main.c
//  Project4
//
//  Created by XuZhisheng on 11/27/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//
#include "Library.h"
#include "initShader.h"
#include <stdio.h>
#include <stdlib.h>
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

//The number of the order of the pair of obstructs
int mage = 0;

//deducer for reconsturcting obstructs
int deducer = -1;

//The starting position for the first vetex of obstructs
int obWhere;

//The array of all the logic center of each obstructs
vec4 obstructs[20];

//The random generator for creating obstructs
time_t timer;

//Whether the game has ended or not. Or even started or not
int star = 0;

//float used to remenber the distance of ball's movement on x-axies
int far = 0;

//float used for jumping measurement
float height = 0.0;

//if jump
int isJump = 0;

//if going down
int isDown = 0;

//All the uniform variable used for lighting model
GLuint ambinent_P;
GLuint diffuse_P;
GLuint specular_P;
GLuint light_Position;
GLuint shiness;
GLuint atten_Constant;
GLuint atten_linear;
GLuint atten_quatric;
GLuint blackC;


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

//The vNormal array will be put into the buffer
vec4 normals[10000];

//The center of the ball
vec4 center;

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

//The variable for my texture
GLuint mytex;

//The variable for enable in vetex shader
GLuint enable_loc;

//The variable for fEnable in fragment shader
GLuint fEnable_loc;

//The array for 2D texture
GLfloat tex_coords[3288-360][2];

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

//The texture array for the image
GLubyte my_texels[1024][1024][3];

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    //Dealing with the 2D texture
    glGenTextures(1, &mytex);
    glBindTexture(GL_TEXTURE_2D, mytex);
    
    FILE *fp;
    unsigned char uc;
    int i, j, k;
    fp = fopen("grass.data", "r");
    if(fp == NULL) {
        printf("Unable to open file\n");
        exit(0); }
    for(i = 0; i < 1024; i++) {
        for(j = 0; j < 1024; j++) {
            for(k = 0; k < 3; k++) {
                fread(&uc, 1, 1, fp);
                my_texels[i][j][k] = uc;
            }
            fread(&uc, 1, 1, fp);
        }
    }
    fclose(fp);
    
    //printf("%i %i %i\n", my_texels[0][0][0], my_texels[0][0][1], my_texels[0][0][2]);
    
    i = 0;
    j = 0;
    k = 0;
    
    /*
    for (int ak = 0; ak < 1024; ak ++)
    {
        for (int fk = 0; fk < 1024; fk ++)
        {
            for (int m = 0; m < 3; m ++)
            {
                printf("%02X ",my_texels[ak][fk][m]);
            }
            
            printf("\n");
        }
    }*/
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    GLuint tex_loc;
    tex_loc = glGetUniformLocation(program, "texture");
    glUniform1i(tex_loc, 0);

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
    
    center = point4(0, 0.18, 0, 1.0);
    black_Color =  point4(0, 0, 0, 1);
    //Initializing the start position for light source
    light_P = point4(0, 2, -2, 1.0);
    
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
        vertices[f] = point4(vertices[f].x/6, vertices[f].y/6+0.18, vertices[f].z/6, 1.0);
    }
    //Done with the sphere
    
    numberOfVertice = g;
    where = g;
    
    
    //Scalling this cube into a planes
    //In-total we will have 20 planes
    for (int j = 0; j < 20; j ++)
    {
        for (int f = 0; f < 36; f ++)
        {
            plane[f] = point4(cube[f].x*10, 0, cube[f].z*2-j, 1.0);
            vertices[numberOfVertice] = plane[f];
            colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
        }
    }
    
    obWhere = numberOfVertice;
    
    //printf("%d\n",numberOfVertice);
    
    srand((unsigned) time(&timer));
    //Creating the initial obstructs for each plane
    for (int akb = 1; akb < 20; akb ++)
    {
        int tempI = rand() % 5;
        
        if (tempI == 0)
        {
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2-2, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
            obstructs[mage++] = point4(-2, 0, 0-akb-0.5, 1.0);
            
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2-1, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(-1, 0, 0-akb-0.5, 1.0);
        }
        else if (tempI == 1)
        {
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2-1, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(-1, 0, 0-akb-0.5, 1.0);
            
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(0, 0, 0-akb-0.5, 1.0);
        }
        else if (tempI == 2)
        {
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(0, 0, 0-akb-0.5, 1.0);
            
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2+1, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(1, 0, 0-akb-0.5, 1.0);
            
        }
        else if (tempI == 3)
        {
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2+1, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(1, 0, 0-akb-0.5, 1.0);
            
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2+2, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
            
             obstructs[mage++] = point4(2, 0, 0-akb-0.5, 1.0);
        }
        else if (tempI == 4)
        {
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2+2, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
            obstructs[mage++] = point4(2, 0, 0-akb-0.5, 1.0);
            
            for (int f = 0; f < 36; f ++)
            {
                vertices[numberOfVertice] = point4(cube[f].x*2-2, cube[f].y*2, cube[f].z*2-akb-0.5, 1.0);
                colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
            }
            
             obstructs[mage++] = point4(-2, 0, 0-akb-0.5, 1.0);
            
        }
        
        akb += 1;
    }
    
    mage = 0;
    
    
    
    
    
    //printf("%d\n",numberOfVertice);
    
    
    //Putting the 2D texture coordinats for all the vertices
    for (int akb = 1128; akb < (3288-360); akb ++)
    {
        tex_coords[akb][0] = 0;
        tex_coords[akb][1] = 1;
        
        akb += 1;
        
        tex_coords[akb][0] = 0;
        tex_coords[akb][1] = 0;
        
        akb += 1;
        
        tex_coords[akb][0] = 1;
        tex_coords[akb][1] = 1;
        
        akb += 1;
        
        tex_coords[akb][0] = 1;
        tex_coords[akb][1] = 1;
        
        akb += 1;
        
        
        tex_coords[akb][0] = 0;
        tex_coords[akb][1] = 0;
        
        akb += 1;
        
        
        tex_coords[akb][0] = 1;
        tex_coords[akb][1] = 0;
        
        
    }
    
    /*
    printf("%f, %f\n", tex_coords[1128][0], tex_coords[1128][1]);
    printf("%f, %f\n", tex_coords[1129][0], tex_coords[1129][1]);
    printf("%f, %f\n", tex_coords[1130][0], tex_coords[1130][1]);
    */
    
    //Calculating the normals
    for (int akb = 0; akb < 1128; akb ++)
    {
        normals[akb] = normalizeVec4(SubV(point4(vertices[akb].x, vertices[akb].y, vertices[akb].z, 1.0), center));
    }
    
    for (int akb = 1128; akb < numberOfVertice; akb ++)
    {
        normals[akb] = point4(0, 1, 0, 0);
    }
    
    
    
    
    //Initializing the rotation matrixs on origin
    matrix_X.x = point4(1, 0, 0, 0);
    matrix_X.y = point4(0, cos(dToR*-5), sin(dToR*-5), 0);
    matrix_X.z = point4(0, -sin(dToR*-5), cos(dToR*-5), 0);
    matrix_X.w = point4(0, 0, 0, 1);
    
    matrix_Y.x = point4(cos(dToR*-4), 0, -sin(dToR*-4), 0);
    matrix_Y.y = point4(0, 1, 0, 0);
    matrix_Y.z = point4(sin(dToR*-4), 0, cos(dToR*-4), 0);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords) + sizeof(normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(tex_coords), tex_coords);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), sizeof(normals), normals);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
    
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(colors) + sizeof(vertices));
    
    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices) + sizeof(colors) + sizeof(tex_coords));
    
    model_location = glGetUniformLocation(program, "model_view_matrix");
    project_location = glGetUniformLocation(program, "projection_matrix");
    ambinent_P = glGetUniformLocation(program, "AmbientProduct");
    diffuse_P = glGetUniformLocation(program, "DiffuseProduct");
    specular_P = glGetUniformLocation(program, "SpecularProduct");
    light_Position = glGetUniformLocation(program, "LightPosition");
    shiness = glGetUniformLocation(program, "shininess");
    atten_Constant = glGetUniformLocation(program, "attenuation_constant");
    atten_linear = glGetUniformLocation(program, "attenuation_linear");
    atten_quatric = glGetUniformLocation(program, "attenuation_quadratic");
    blackC = glGetUniformLocation(program, "black");
    enable_loc = glGetUniformLocation(program, "enable");
    fEnable_loc= glGetUniformLocation(program, "fEnable");
    
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    
    
}//end of init


void display(void)
{
    
    
    glClearDepth(1);
    
    glClearColor (1,1,1,1.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    glUniformMatrix4fv(project_location, 1, GL_FALSE, (GLfloat *) &tr2_matrix);
    glUniform4fv(light_Position, 1, (GLfloat *) &light_P);
    glUniform4fv(blackC, 1, (GLfloat *) &black_Color);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    glUniform1i(enable_loc, 1);
    glUniform1i(fEnable_loc, 1);
    
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
    
    glDrawArrays(GL_TRIANGLES, 0, 1128);
    
    
    //The shadow for the ball
    glUniform1i(enable_loc, 3);
    glUniform1i(fEnable_loc, 1);
    
    glDrawArrays(GL_TRIANGLES, 0, 1128);
    
    
    //The obstructs
    glUniform1i(enable_loc, 4);
    glUniform1i(fEnable_loc, 1);
    
    glDrawArrays(GL_TRIANGLES, 1848, numberOfVertice);
    
    //The planes
    glUniform1i(enable_loc, 2);
    glUniform1i(fEnable_loc, 2);
    
    glDrawArrays(GL_TRIANGLES, 1128, 1848);
    
    glutSwapBuffers();
}//end of display



void keyboard(unsigned char key, int mousex, int mousey)
{
    
    
    if (key == 'q' || key == 'Q')
    {
        exit(0);
    }
    else if (key == 'a' || key == 'A')//moving the ball to the left
    {
        
        center = point4(center.x-0.15, center.y, center.z, 1.0);
        far += 1;
        
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f] = point4(vertices[f].x - 0.15, vertices[f].y, vertices[f].z, 1.0);
            
            vertices[f] = point4(vertices[f].x + (0.15*far), vertices[f].y-center.y, vertices[f].z, 1.0);
            vertices[f] = pointMulti44Times41(matrix_X, vertices[f]);
            vertices[f] = pointMulti44Times41(matrix_Y, vertices[f]);
            vertices[f] = point4(vertices[f].x - (0.15*far), vertices[f].y+center.y, vertices[f].z, 1.0);
            
            normals[f] = normalizeVec4(SubV(vertices[f], center));
            
        }
    }
    else if (key == 'd' || key == 'D')//moving the ball to the right
    {
        
        center = point4(center.x+0.15, center.y, center.z, 1.0);
        far -= 1;
        
        for (int f = 0; f < 1128; f ++)
        {
            vertices[f] = point4(vertices[f].x + 0.15, vertices[f].y, vertices[f].z, 1.0);
            
            vertices[f] = point4(vertices[f].x + (0.15*far), vertices[f].y-center.y, vertices[f].z, 1.0);
            vertices[f] = pointMulti44Times41(matrix_X, vertices[f]);
            vertices[f] = pointMulti44Times41(matrix_Y, vertices[f]);
            vertices[f] = point4(vertices[f].x - (0.15*far), vertices[f].y+center.y, vertices[f].z, 1.0);
            
            normals[f] = normalizeVec4(SubV(vertices[f], center));
            
        }
    }
    else if (key == 32)//jumping
    {
        if (isDown == 1)
        {
            
        }
        else
        {
            isJump = 1;
        }
    }
    else if (key == 'k' || key == 'K')
    {
        star = 1;
    }
    
    
    if (star == 1)
    {
        //To check if the ball is out of boundary or not
        if (center.x < -2.5 || center.x > 2.5)
        {
            star = 0;
            printf("\nMan, your ball falls into the invisible abyss");
        }
        
        
        //Collision detection
        for (int akb = 0; akb < 20; akb ++)
        {
            if (distance(obstructs[akb], center) <= (0.17 + 0.5))
            {
                star = 0;
                printf("\nThe collision happens. Boom!\n");
                break;
            }
            else
            {
                continue;
            }
        }
    }
    
    
    
    
    tr_matrix = look_at(eyeP.x, eyeP.y, eyeP.z, centerP.x, centerP.y, centerP.z, 0.0, 1.0, 0.0);
    tr2_matrix = frustum(-0.4, 0.4, -0.4, 0.4, -0.4,-100);

    
    //Repush the new modified array back into the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), sizeof(normals), normals);
    
    glutPostRedisplay();
    
}//end of keyboard


void mouse(int button, int state, int x, int y)
{
    
    
    
    
}//end of mouse function





void idle (void)
{
    
    if (star == 1)
    {
        //printf("%.f\n",);
        
        //apply the rotation matrix for all vertices for the sphere
        for (int f = 0; f < g; f ++)
        {
            vertices[f] = point4(vertices[f].x, vertices[f].y-center.y, vertices[f].z, 1.0);
            vertices[f] = pointMulti44Times41(matrix_X, vertices[f]);
            vertices[f] = point4(vertices[f].x, vertices[f].y+center.y, vertices[f].z, 1.0);
            
            normals[f] = normalizeVec4(SubV(point4(vertices[f].x, vertices[f].y, vertices[f].z, 1.0), center));
        }
        
        if (isJump == 1)//jumping up the ball
        {
            center = point4(center.x, center.y+0.02, center.z, 1.0);
            height += 0.02;
            
            //printf("in\n");
            
            for (int f = 0; f < 1128; f ++)
            {
                vertices[f] = point4(vertices[f].x, vertices[f].y + 0.02, vertices[f].z, 1.0);
                normals[f] =  normalizeVec4(SubV(vertices[f], center));
            }
            
            if (height >= 0.6)
            {
                //printf("%.f\n in up",height);
                isJump = 0;
                isDown = 1;
            }
        }
        
        
        if (isDown == 1)//pulling down the ball
        {
            center = point4(center.x, center.y - 0.02, center.z, 1.0);
            height -= 0.02;
            //printf("out\n");
            for (int f = 0; f < 1128; f ++)
            {
                vertices[f] = point4(vertices[f].x, vertices[f].y - 0.02, vertices[f].z, 1.0);
                normals[f] =  normalizeVec4(SubV(vertices[f], center));
            }
            
            if (center.y <= 0.18)
            {
                isJump = 0;
                isDown = 0;
                height = 0;
                
                //printf("%.f in down\n",center.y);
            }
            
        }
        
        
        //Moving all the planes
        for (int f = g; f < numberOfVertice; f ++)
        {
            vertices[f] = point4(vertices[f].x, vertices[f].y, vertices[f].z+0.02, 1.0);
        }
        
        
        //Moving the logic position of obstruct
        for (int akb = 0; akb < 20; akb ++)
        {
            obstructs[akb] = point4(obstructs[akb].x, obstructs[akb].y, obstructs[akb].z + 0.02, 1.0);
        }
        
        test = point4(0, 0, test.z+0.02, 1.0);
        
        count += 0.02;
        
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
            
            //Moving back the current plane
            for (int f = where; f < (where + 36); f ++)
            {
                vertices[f] = point4(vertices[f].x, vertices[f].y, vertices[f].z-20, 1.0);
            }
            
            
            //Reconstructing the obstructs
            if (which % 2 == 1)
            {
                
                srand((unsigned) time(&timer));
                int tempI = rand() % 5;
                
                int tempC1 = 0;
                int tempC2 = 0;
                
                if (tempI == 0)
                {
                    for (int f = obWhere + ((which + deducer) * 72); f < (obWhere + ((which + deducer) * 72) + 72); f ++)
                    {
                        if (tempC1 < 36)
                        {
                            vertices[f] = point4(cube[tempC1].x*2-2, cube[tempC1].y*2, cube[tempC1].z*2-19.5, 1.0);
                            tempC1 ++;
                        }
                        else if (tempC2 < 36)
                        {
                            vertices[f] = point4(cube[tempC2].x*2-1, cube[tempC2].y*2, cube[tempC2].z*2-19.5, 1.0);
                            tempC2 ++;
                        }
                    }
                    
                    obstructs[mage++] = point4(-2, 0, -19.5, 1.0);
                    obstructs[mage++] = point4(-1, 0, -19.5, 1.0);
                }
                else if (tempI == 1)
                {
                    for (int f = obWhere + ((which + deducer) * 72); f < (obWhere + ((which + deducer) * 72) + 72); f ++)
                    {
                        if (tempC1 < 36)
                        {
                            vertices[f] = point4(cube[tempC1].x*2, cube[tempC1].y*2, cube[tempC1].z*2-19.5, 1.0);
                            tempC1 ++;
                        }
                        else if (tempC2 < 36)
                        {
                            vertices[f] = point4(cube[tempC2].x*2-1, cube[tempC2].y*2, cube[tempC2].z*2-19.5, 1.0);
                            tempC2 ++;
                        }
                    }
                    
                    
                    obstructs[mage++] = point4(0, 0, -19.5, 1.0);
                    obstructs[mage++] = point4(-1, 0, -19.5, 1.0);
                }
                else if (tempI == 2)
                {
                    for (int f = obWhere + ((which + deducer) * 72); f < (obWhere + ((which + deducer) * 72) + 72); f ++)
                    {
                        if (tempC1 < 36)
                        {
                            vertices[f] = point4(cube[tempC1].x*2, cube[tempC1].y*2, cube[tempC1].z*2-19.5, 1.0);
                            tempC1 ++;
                        }
                        else if (tempC2 < 36)
                        {
                            vertices[f] = point4(cube[tempC2].x*2+1, cube[tempC2].y*2, cube[tempC2].z*2-19.5, 1.0);
                            tempC2 ++;
                        }
                    }
                    
                    obstructs[mage++] = point4(0, 0, -19.5, 1.0);
                    obstructs[mage++] = point4(1, 0, -19.5, 1.0);
                }
                else if (tempI == 3)
                {
                    for (int f = obWhere + ((which + deducer) * 72); f < (obWhere + ((which + deducer) * 72) + 72); f ++)
                    {
                        if (tempC1 < 36)
                        {
                            vertices[f] = point4(cube[tempC1].x*2+2, cube[tempC1].y*2, cube[tempC1].z*2-19.5, 1.0);
                            tempC1 ++;
                        }
                        else if (tempC2 < 36)
                        {
                            vertices[f] = point4(cube[tempC2].x*2+1, cube[tempC2].y*2, cube[tempC2].z*2-19.5, 1.0);
                            tempC2 ++;
                        }
                    }
                    
                    
                    obstructs[mage++] = point4(2, 0, -19.5, 1.0);
                    obstructs[mage++] = point4(1, 0, -19.5, 1.0);
                }
                else if (tempI == 4)
                {
                    for (int f = obWhere + ((which + deducer) * 72); f < (obWhere + ((which + deducer) * 72) + 72); f ++)
                    {
                        if (tempC1 < 36)
                        {
                            vertices[f] = point4(cube[tempC1].x*2-2, cube[tempC1].y*2, cube[tempC1].z*2-19.5, 1.0);
                            tempC1 ++;
                        }
                        else if (tempC2 < 36)
                        {
                            vertices[f] = point4(cube[tempC2].x*2+2, cube[tempC2].y*2, cube[tempC2].z*2-19.5, 1.0);
                            tempC2 ++;
                        }
                    }
                    
                    obstructs[mage++] = point4(-2, 0, -19.5, 1.0);
                    obstructs[mage++] = point4(2, 0, -19.5, 1.0);
                }
                
                
                
                
                deducer -= 1;
            }
            
            
            which += 1;
            
            where += 36;
            
            //check if current plane is already the last plane
            if (which == 20)
            {
                where = g;
                which = 0;
                mage = 0;
                deducer = -1;
            }
        }
    }
    
    //To check if the ball falls or not
    if (star == 1)
    {
        //To check if the ball is out of boundary or not
        if (center.x < -2.5 || center.x > 2.5)
        {
            star = 0;
            printf("\nMan, your ball falls into the invisible abyss");
        }
        
        
        
        //Collision detection
        for (int akb = 0; akb < 20; akb ++)
        {
            if (distance(obstructs[akb], center) <= (0.17 + 0.5))
            {
                star = 0;
                printf("\nThe collision happens. Boom!\n");
                break;
            }
            else
            {
                continue;
            }
        }
    }
    
    
    
    
    
    
    
    
    tr_matrix = look_at(eyeP.x, eyeP.y, eyeP.z, centerP.x, centerP.y, centerP.z, 0.0, 1.0, 0.0);
    tr2_matrix = frustum(-0.5, 0.5, -0.5, 0.5, -0.45,-100);
    
    //Repush the new modified array back into the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), sizeof(normals), normals);
    
    glutPostRedisplay();
}//end of idle function


int main(int argc, char **argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project4 ZhishengXu");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}//end of main


