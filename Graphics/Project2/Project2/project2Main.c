//
//  main.c
//  Project2
//
//  Created by XuZhisheng on 9/28/17.
//  Copyright © 2017 XuZhisheng. All rights reserved.
//
#include "genMaze.h"
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

//The enum for facing when solving the maze
enum Faceing{South,North,East,West}face;

//The varible used to control if the rotation of camera has been completed or not
float change;

//The prototype for the maze cells
cell *cells;

//The countdown for flying around the maze
GLfloat countDown;

//The flag used to control wheter the flying has been finished or not
int enable;

//The flag used to control the fly to the entrance of the maze
int toMazeEntrance;

//The position of eye
vec4 eyeP;

//The position of the center of the maze
vec4 centerP;

//The vector from the eye point to the entrance of the maze
vec4 toEntrance;

//The countdown for the flying to the maze
float toECount;

//Whether we need to move into the maze or not
int flag;

//Whether we begin to solve the maze or not
int solve;

//The logic location of the "Person" who traveses through the maze
vec4 lp;

//The logic position of the person in the 2D array
int whereX;

int whereY;

//If the first movement has been completed or not
int firstD = 0;

//Rotation matrix for cube 1
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

//The color array will be put into the buffer
vec4 colors[7000];

//The actual vector that is going to be put into the  buffer
vec4 vertices[7000];

//Our original point (0,0,0,1)
vec4 origin;

//The basic 0.5X0.5X0.5 cube
vec4 cube[36];

//The wall 0.25*1.25*1
vec4 walls[36];

//The pole 0.25*1.5*0.25
vec4 poles[36];

//The ground for the maze
vec4 ground[36];

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
    srand(time(0));
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    
    numberOfVertice = 0;
    
    //Initializing the origin
    origin = point4(0.0, 0.0, 0.0, 1.0);
    
    //Initializing the cells
    cells = (cell *) malloc(sizeof(cell) * 8 * 8);
    memset(cells, 0, sizeof(cell) * 8 * 8);
    
    gen_maze(8, 8, cells);
    //casting the cells into a two-D array
    cell (*cells2D)[8] = (cell (*)[8]) cells;
    
    
    print_maze(8, 8, cells2D);
    printf("%d",get_num_walls(8, 8, cells2D));
    
    
    
    //Initilizaing some variables
    toECount = 0.0;
    countDown = 0.0;
    enable = 1;
    toMazeEntrance = 0;
    change = 0.0;
    flag = 0;
    solve = 0;
    
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
    
    
    //Scalling this cube into a wall, pole and ground
    for (int k = 0; k < 36; k ++)
    {
        walls[k] = point4(cube[k].x/2, cube[k].y*2.5, cube[k].z*2, 1.0);
        poles[k] = point4(cube[k].x/2, cube[k].y*3, cube[k].z/2, 1.0);
        ground[k] = point4((cube[k].x-0.1)*5000, (cube[k].y - 0.3)*20, (cube[k].z)*5000, 1.0);
        //vertices[k] = walls[k];
    }
    
    
    
    
    //Initializing the rotation matrixs on origin
    matrix_X.x = point4(1, 0, 0, 0);
    matrix_X.y = point4(0, cos(dToR*3), sin(dToR*3), 0);
    matrix_X.z = point4(0, -sin(dToR*3), cos(dToR*3), 0);
    matrix_X.w = point4(0, 0, 0, 1);
    
    matrix_Y.x = point4(cos(dToR*90), 0, -sin(dToR*90), 0);
    matrix_Y.y = point4(0, 1, 0, 0);
    matrix_Y.z = point4(sin(dToR*90), 0, cos(dToR*90), 0);
    matrix_Y.w = point4(0, 0, 0, 1);
    
    matrix_Z.x = point4(cos(dToR*3), sin(dToR*3), 0, 0);
    matrix_Z.y = point4(-sin(dToR*3), cos(dToR*3), 0, 0);
    matrix_Z.z = point4(0, 0, 1, 0);
    matrix_Z.w = point4(0, 0, 0, 1);

    flying_Y.x = point4(cos(dToR*0.4), 0, -sin(dToR*0.4), 0);
    flying_Y.y = point4(0, 1, 0, 0);
    flying_Y.z = point4(sin(dToR*0.4), 0, cos(dToR*0.4), 0);
    flying_Y.w = point4(0, 0, 0, 1);
    
    //Initilizaing the eyeP and centerP
    eyeP = point4(-1.25, 6, -3.75, 1.0);
    centerP = point4(5, 0, 3.75, 1.0);
    
    //Putting some random colors for the walls
    //Poles will be pure blue
    /*
    for (int akb = 0; akb < 36; akb ++)
    {
        colors[akb] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
    }
    */
    
    //There always going to be 81 poles and 79 walls
    //Begin to calculate the actual position for poles and walls
    
    GLfloat oldX = 0;
    GLfloat oldZ = 0;
    //Y is always zero
    
    //putting the walls first
    
    for (int k = 0; k < 8; k ++)
    {
        GLfloat tempX = oldX;
        GLfloat tempZ = oldZ;
        for (int h = 0; h < 8; h ++)
        {
            if (cells2D[k][h].west == 1)
            {
                for (int akb = 0; akb < 36; akb ++)
                {
                    vertices[numberOfVertice] = AddiV(walls[akb], point4(oldX-0.625, 0, oldZ, 0));
                    colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
                }
            }
            if (cells2D[k][h].north == 1)
            {
                for (int akb = 0; akb < 36; akb ++)
                {
                    
                    vec4 temp = pointMulti44Times41(matrix_Y, walls[akb]);
                    vertices[numberOfVertice] = AddiV(temp, point4(oldX, 0, oldZ-0.625, 0));
                    colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
                }
            }
            if (cells2D[k][h].east == 1 && h == 7)
            {
                
                for (int akb = 0; akb < 36; akb ++)
                {
                    vertices[numberOfVertice] = AddiV(walls[akb], point4(oldX+0.625, 0, oldZ, 0));
                    colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
                }
            }
            if (cells2D[k][h].south == 1 && k == 7)
            {
                for (int akb = 0; akb < 36; akb ++)
                {
                    
                    vec4 temp = pointMulti44Times41(matrix_Y, walls[akb]);
                    vertices[numberOfVertice] = AddiV(temp, point4(oldX, 0, oldZ+0.625, 0));
                    colors[numberOfVertice++] = point4((rand()%254)/255.0, (rand()%254)/255.0, (rand()%254)/255.0, 1.0);
                }
            }
            
            oldX += 1.25;
            
        }//end of h loop
        
        oldX = tempX;
        oldZ = tempZ;
        
        oldZ += 1.25;
    }//end of k loop
    
    oldX = -0.625;
    oldZ = -0.625;
    
    //Putting the poles after putting the walls
    for (int k = 0; k < 9; k ++)
    {
        GLfloat tempX = oldX;
        GLfloat tempZ = oldZ;
        for (int h = 0; h < 9; h ++)
        {
            for (int akb = 0; akb < 36; akb ++)
            {
                vertices[numberOfVertice] = AddiV(poles[akb], point4(oldX, 0, oldZ, 0));
                colors[numberOfVertice++] = point4(0, 0, 1, 1);
            }
            oldX += 1.25;
        }
        
        oldX = tempX;
        oldZ = tempZ;
        
        oldZ += 1.25;
    }
    
    
    
    
    
    //Putting in the ground cube
    for (int k = 0; k < 36; k ++)
    {
        vertices[numberOfVertice] = ground[k];
        colors[numberOfVertice++] = point4(1, 0, 0, 1);
    }
    
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
    
    printf("\n%d",numberOfVertice);
    
}//end of init


void display(void)
{
    
    
    glClearDepth(1);
    
    glClearColor (0,1,1,1.0);
    
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
    if (key == 'd')
    {
        vec4 toO = SubV(origin, point4(5, 0, 3.75, 1.0));
        eyeP = AddiV(eyeP, toO);
        eyeP = pointMulti44Times41(flying_Y, eyeP);
        vec4 toEye = SubV(point4(5, 0, 3.75, 1.0), origin);
        eyeP = AddiV(eyeP, toEye);
    }
    if (key == 'g')
    {
        toMazeEntrance = 1;
        toEntrance = SubV(point4(-1.875, 0.1, 0, 1.0), eyeP);
    }
    if (key == 'f')
    {
        flag = 1;
    }
    if (key == 's')
    {
        va -= 0.1;
        printf("\n%.3f",va);
    }
    if (key == 'w')
    {
        va += 0.1;
        printf("\n%.3f",va);
    }
    
}//end of keyboard


void mouse(int button, int state, int x, int y)
{
    
    
    
    
}//end of mouse function





void idle (void)
{
    
    //tr_matrix = MultiMat4(matrix_Y, tr_matrix);
    //tr_matrix = look_at(xV, yV, zV,1.25, 0, 1.25, 0, 1, 0);
    //tr_matrix = look_at(-1.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    //Flying around the maze
    if (enable == 1)
    {
        vec4 toO = SubV(origin, centerP);
        eyeP = AddiV(eyeP, toO);
        eyeP = pointMulti44Times41(flying_Y, eyeP);
        vec4 toEye = SubV(centerP, origin);
        eyeP = AddiV(eyeP, toEye);
        if (countDown >= 360)
        {
            enable = 0;
        }
    }
    
    //Moving the eye to the position where we can have a clear site of the entrance
    if (toMazeEntrance == 1)
    {
        eyeP = AddiV(eyeP, point4(toEntrance.x*0.01, toEntrance.y*0.01, toEntrance.z*0.01, 0));
        centerP = point4(0.625, 0.1, 0, 1.0);
        toECount += 0.01;

        if (toECount >= 1.0)
        {
            toMazeEntrance = 0;
        }
    }
    
    //Move the eye to the initial position when solving maze
    //Set the default direction as east, default logic location at 0,0,0
    if (flag == 1)
    {
        
        eyeP = AddiV(eyeP, point4(1.25*0.01, 0, 0, 0));
        
        change += 0.01;
        
        if (change >= 1)
        {
            change = 0;
            flag = 0;
            solve = 1;
            lp = point4(0, 0.1, 0, 1.0);
            whereX = 0;
            whereY = 0;
            face = East;
        }
        
    }
    
    
    //casting the cells into a two-D array
    cell (*cells2D)[8] = (cell (*)[8]) cells;
    
    //The temp vectors used to control the movement of camera(eye)
    vec4 tempE;
    vec4 tempC;
    
    
    //The actual solving process for the maze
    //Currently using the right hand rule
    if (solve == 1)
    {
        switch (face) {
            case East:
                if (cells2D[whereX][whereY].south == 0)//no wall on the right
                {
                    
                    if (firstD == 0)
                    {
                        //printf("TTTTTTT\n");
                        tempE = SubV(point4(lp.x, lp.y, lp.z-0.625, 1.0), point4(lp.x-0.625, lp.y, lp.z, 1.0));
                        tempC = SubV(point4(lp.x, lp.y, lp.z+0.625, 1.0), point4(lp.x+0.625, lp.y, lp.z, 1.0));
                        
                        eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                        centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                        
                        change += 0.01;
                        if (change >= 1)
                        {
                            firstD = 1;
                            change = 0;
                            
                        }
                        
                    }
                    else if (firstD == 1)
                    {
                        
                        eyeP = AddiV(eyeP, point4(0, 0, 1.25*0.01, 0));
                        centerP = AddiV(centerP, point4(0, 0, 1.25*0.01, 0));
                        change += 0.01;
                        
                        if (change >= 1)
                        {
                            firstD = 0;
                            change = 0;
                            face = South;
                            lp.z += 1.25;
                            whereX += 1;
                        }
                    }
                    
                }
                else if (cells2D[whereX][whereY].south == 1 && cells2D[whereX][whereY].east == 1)//wall on right and front
                {
                    //printf("Turning left\n");
                    tempE = SubV(point4(lp.x, lp.y, lp.z+0.625, 1.0), point4(lp.x-0.625, lp.y, lp.z, 1.0));
                    tempC = SubV(point4(lp.x, lp.y, lp.z-0.625, 1.0), point4(lp.x+0.625, lp.y, lp.z, 1.0));
                    
                    eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0.0));
                    centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0.0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = North;
                        
                    }
                }
                else if (cells2D[whereX][whereY].south == 1 && cells2D[whereX][whereY].east == 0)// wall on right not front
                {
                    
                    lp.x += 1.25;
                    eyeP = AddiV(eyeP, point4(1.25*0.01, 0, 0, 0.0));
                    centerP = AddiV(centerP, point4(1.25*0.01, 0, 0, 0));
                    change += 0.01;
                    if (change >= 1)
                    {
                        change = 0;
                        whereY += 1;
                        face = East;
                    }
                    
                }
                break;//end of East
                
            case South:
                if (cells2D[whereX][whereY].west == 0)//no wall on the right
                {
                    if (firstD == 0)
                    {
                        tempE = SubV(point4(lp.x+0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z-0.625, 1.0));
                        tempC = SubV(point4(lp.x-0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z+0.625, 1.0));
                        
                        
                        eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                        centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                        
                        change += 0.01;
                        if (change >= 1)
                        {
                            firstD = 1;
                            change = 0;
                            
                        }
                        
                    }
                    else if (firstD == 1)
                    {
                        eyeP = AddiV(eyeP, point4(-1.25*0.01, 0, 0, 0));
                        centerP = AddiV(centerP, point4(-1.25*0.01, 0, 0, 0));
                        change += 0.01;
                        
                        if (change >= 1)
                        {
                            change = 0;
                            firstD = 0;
                            face = West;
                            lp.x -= 1.25;
                            whereY -= 1;
                        }
                    }
                }
                else if (cells2D[whereX][whereY].south == 1 && cells2D[whereX][whereY].west == 1)//wall on front and right
                {
                    tempE = SubV(point4(lp.x-0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z-0.625, 1.0));
                    tempC = SubV(point4(lp.x+0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z+0.625, 1.0));
                    
                    eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                    centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = East;
                    }
                }
                else if (cells2D[whereX][whereY].west == 1 && cells2D[whereX][whereY].south == 0)//wall on right not front
                {
                    eyeP = AddiV(eyeP, point4(0, 0, 1.25*0.01, 0));
                    centerP = AddiV(centerP, point4(0, 0, 1.25*0.01, 0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = South;
                        whereX += 1;
                        lp.z += 1.25;
                    }
                }
                break;//end of south
                
            case West:
                if (cells2D[whereX][whereY].north == 0)//no wall in right
                {
                    if (firstD == 0)
                    {
                        tempE = SubV(point4(lp.x, lp.y, lp.z+0.625, 1.0), point4(lp.x+0.625, lp.y, lp.z, 1.0));
                        tempC = SubV(point4(lp.x, lp.y, lp.z-0.625, 1.0), point4(lp.x-0.625, lp.y, lp.z, 1.0));
                        
                        
                        eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                        centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                        
                        change += 0.01;
                        if (change >= 1)
                        {
                            firstD = 1;
                            change = 0;
                            
                        }
                        
                    }
                    else if (firstD == 1)
                    {
                        eyeP = AddiV(eyeP, point4(0, 0, -1.25*0.01, 0));
                        centerP = AddiV(centerP, point4(0, 0, -1.25*0.01, 0));
                        change += 0.01;
                        
                        if (change >= 1)
                        {
                            change = 0;
                            firstD = 0;
                            face = North;
                            lp.z -= 1.25;
                            whereX -= 1;
                        }
                    }
                }
                else if (cells2D[whereX][whereY].west == 1 && cells2D[whereX][whereY].north == 1)//wall on front and right
                {
                    tempE = SubV(point4(lp.x, lp.y, lp.z-0.625, 1.0), point4(lp.x+0.625, lp.y, lp.z, 1.0));
                    tempC = SubV(point4(lp.x, lp.y, lp.z+0.625, 1.0), point4(lp.x-0.625, lp.y, lp.z, 1.0));
                    
                    
                    eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                    centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = South;
                    }
                    
                }
                else if (cells2D[whereX][whereY].west == 0 && cells2D[whereX][whereY].north == 1)//wall on right not front
                {
                    eyeP = AddiV(eyeP, point4(-1.25*0.01, 0, 0, 0));
                    centerP = AddiV(centerP, point4(-1.25*0.01, 0, 0, 0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = West;
                        whereY -= 1;
                        lp.x -= 1.25;
                    }
                }
                break;//end of west
                
                
            case North:
                if (cells2D[whereX][whereY].east == 0)//no wall on the right
                {
                    if (firstD == 0)
                    {
                        tempE = SubV(point4(lp.x-0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z+0.625, 1.0));
                        tempC = SubV(point4(lp.x+0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z-0.625, 1.0));
                        
                        
                        eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                        centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                        
                        change += 0.01;
                        if (change >= 1)
                        {
                            firstD = 1;
                            change = 0;
                            
                        }
                        
                    }
                    else if (firstD == 1)
                    {
                        eyeP = AddiV(eyeP, point4(1.25*0.01, 0, 0, 0));
                        centerP = AddiV(centerP, point4(1.25*0.01, 0, 0, 0));
                        change += 0.01;
                        
                        if (change >= 1)
                        {
                            change = 0;
                            firstD = 0;
                            face = East;
                            lp.x += 1.25;
                            whereY += 1;
                        }
                    }
                }
                else if (cells2D[whereX][whereY].north == 1 && cells2D[whereX][whereY].east == 1)//wall on front and right
                {
                    tempE = SubV(point4(lp.x + 0.625, lp.y, lp.z, 1.0), point4(lp.x, lp.y, lp.z+0.625, 1.0));
                    tempC = SubV(point4(lp.x-0.625, lp.y, lp.z, 1.0),point4(lp.x, lp.y, lp.z-0.625, 1.0));
                    
                    eyeP = AddiV(eyeP, point4(tempE.x*0.01, 0, tempE.z*0.01, 0));
                    centerP = AddiV(centerP, point4(tempC.x*0.01, 0, tempC.z*0.01, 0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = West;
                    }
                    
                }
                else if (cells2D[whereX][whereY].north == 0 && cells2D[whereX][whereY].east == 1)//wall on right not front
                {
                    eyeP = AddiV(eyeP, point4(0, 0, -1.25*0.01, 0));
                    centerP = AddiV(centerP, point4(0, 0, -1.25*0.01, 0));
                    change += 0.01;
                    
                    if (change >= 1)
                    {
                        change = 0;
                        face = North;
                        whereX -= 1;
                        lp.z -= 1.25;
                    }
                }
                break;//end of North
            default:
                printf("what the fuck is that?\n");
                break;
                
        }//end of switch
        
        if (whereX == 7 && whereY == 8)//we solve the maze
        {
            printf("\nCongragulations!!!!!\n");
            solve = 0;
        }
        
    }//end of solve
    
    
    tr_matrix = look_at(eyeP.x, eyeP.y, eyeP.z, centerP.x, centerP.y, centerP.z, 0.0, 1.0, 0.0);  //model view
    countDown += 0.4;
    tr2_matrix = frustum(-0.3-va, 0.3+va, -0.3-va, 0.3+va, -0.3-va,-100);         //projection when walking through the maze
    
    //tr2_matrix = frustum(-1.5, 1.5, -1.5, 1.5, -1.5,-100);
    

    
    glutPostRedisplay();
}//end of idle function


int main(int argc, char **argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project2 ZhishengXu");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}//end of main
