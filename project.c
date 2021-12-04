#include "CSCIx229.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdbool.h>
#include <time.h>
float pitch=0.0, yaw=0.0;
float camX=0.0, camZ=0.0;
#define MAX_PARTICLES 2000
#define WCX 640
#define WCY 480
#define RAIN 0
#define SNOW 1
#define HAIL 2
int fog =0;
int sno=0;

float slowdown = 2.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1/4;
float cx =0,cz=0;
float ccx =0,ccz=0;
float czz =0,cccz=0;
int loop;
int fall;
int an;
int k=0;
int kk=0;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

typedef struct {
  // Life
  bool alive; // is the particle alive?
  float life; // particle lifespan
  float fade; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // Velocity/Direction, only goes down in y dir
  float vel;
  // Gravity
  float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

int th=0;          //  Azimuth of view angle
int ph=0;          //  Elevation of view angle
int ntex=0;
int zh=90;
float lx=0.0f,lz=0.1f , ly = 0.0 ;
float angle=0.0;
float x1=1.0f,z1=1.0f;
float s=1.0;      
int axes=1;        //  Display axes
int mode=1;
int move=1;        // Mode of projection
int fov=55;
int obj=0;
double asp=1;
double dim=5.0;
double rep =1;
int pro =1;
int light = 1;
int one = 1;
int distance= 5;
int inc = 10;
int smooth = 1;
int local =0;
int emission=0;
int ambient=10;
int diffuse=50;
int specular=0;
int shininess=0;
float shiny=1;
int ch= 90;
int con=0;
float xlight=0;
float ylight=0;
float zlight=0;
typedef struct{float x,y,z;} vtx;
typedef struct{int A,B,C;} tri;
#define n 500
vtx is[n];
#define PI 3.1452
unsigned int texture[40];

//static void Vertex(double th,double ph)
//{
//   double x = Sin(th)*Cos(ph);
//   double y = Cos(th)*Cos(ph);
//   double z =         Sin(ph);
//   //  For a sphere at the origin, the position
//   //  and normal vectors are the same
//   glNormal3d(x,y,z);
//   glTexCoord2d(th/360.0,ph/180.0+0.5);
//   glVertex3d(x,y,z);
//}
void trunk1(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius,
                   GLfloat height)
{  
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

//  Enable textures
   
glColor3f(0.5f, 0.35f, 0.05f);
glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   
glBegin(GL_QUAD_STRIP);
    angle = atan(radius/height);
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(cos(angle),0, sin(angle) );
            glTexCoord2f(0.5*x,0.5*y);
            glVertex3f(xl+x, height+z,yl+y);
            glVertex3f(xl+x, 0.0+z , yl+y);
 
           // glVertex3f(x, y , height);
           // glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
  glVertex3f(xl+radius, height+z,0.0+yl);
        glVertex3f(radius+xl, 0.0+z, 0.0+yl);
     
// glVertex3f(radius, 0.0, height);
 //       glVertex3f(radius, 0.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    /** Drawing the circle on top of cylinder */
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
glVertex3f(x+xl, height+z,y+yl);
           
// glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
glVertex3f(radius+xl,  height+z,0.0+yl);
       
// glVertex3f(radius, 0.0, height);
    glEnd();
}


void tree1(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius,
                   GLfloat height)
{  
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
   

    glColor3f(0,1,0);
    glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);

   glBindTexture(GL_TEXTURE_2D,texture[3]);
//if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(cos(angle),0, sin(angle) );
            glTexCoord2f(x,y);
            glVertex3f(xl, height+z,yl);
            glVertex3f(xl+x, 0.0 +z, yl+y);

           // glVertex3f(x, y , height);
           // glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
  glVertex3f(xl, height+z,yl);
        glVertex3f(radius+xl, 0.0+z, 0.0+yl);

// glVertex3f(radius, 0.0, height);
 //       glVertex3f(radius, 0.0, 0.0);
    glEnd();
glDisable(GL_TEXTURE_2D);
}

void trunk2(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius,
                   GLfloat height)
{
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

glColor3f(0.5f, 0.35f, 0.05f);
glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
//   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   
glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(cos(angle),0, sin(angle));
            glTexCoord2f(x,y);
            glVertex3f(xl+x, height+z,yl+y);
            glVertex3f(xl+x, 0.0+z , yl+y);
 
           // glVertex3f(x, y , height);
           // glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
  glVertex3f(xl+radius, height+z,0.0+yl);
        glVertex3f(radius+xl, 0.0+z, 0.0+yl);
     
// glVertex3f(radius, 0.0, height);
 //       glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Drawing the circle on top of cylinder */
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
glVertex3f(x+xl, height+z,y+yl);
           
// glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
glVertex3f(radius+xl,  height+z,0.0+yl);
       
// glVertex3f(radius, 0.0, height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void tree2(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius,
                   GLfloat height)
{
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    glColor3f(0,1,0);
    glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
//   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(cos(angle),0, sin(angle));
            glTexCoord2f(x,y);
            glVertex3f(xl, height+z,yl);
            glVertex3f(xl+x, 0.0 +z, yl+y);

           // glVertex3f(x, y , height);
           // glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
  glVertex3f(xl, height+z,yl);
        glVertex3f(radius+xl, 0.0+z, 0.0+yl);

// glVertex3f(radius, 0.0, height);
 //       glVertex3f(radius, 0.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
void roof(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius,
                   GLfloat height)
{
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    glColor3f(0.5,1,0.5);
    glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
           
            glNormal3f(cos(angle), 0,sin(angle));
            glTexCoord2f(x,y);
            glVertex3f(xl, height+z,yl);
            glVertex3f(xl+x, 0.0 +z, yl+y);

           // glVertex3f(x, y , height);
           // glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
  glVertex3f(xl, height+z,yl);
        glVertex3f(radius+xl, 0.0+z, 0.0+yl);

// glVertex3f(radius, 0.0, height);
 //       glVertex3f(radius, 0.0, 0.0);
    glEnd();
glDisable(GL_TEXTURE_2D);
}


void draw_cone(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius,
                   GLfloat height)
{
    GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    glColor3f(0,1,0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
glVertex3f(xl, height+z,yl);
            glVertex3f(xl+x, 0.0 +z, yl+y);

           // glVertex3f(x, y , height);
           // glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
  glVertex3f(xl, height+z,yl);
        glVertex3f(radius+xl, 0.0+z, 0.0+yl);

// glVertex3f(radius, 0.0, height);
 //       glVertex3f(radius, 0.0, 0.0);
    glEnd();

}




//static void sphere2(double x,double y,double z,double r)
//{
//   const int d=15;
//
//   //  Save transformation
//   glPushMatrix();
//   //  Offset and scale
//   glTranslated(x,y,z);
//   glScaled(r,r,r);
//
//   //  Latitude bands
//   for (int ph=-90;ph<90;ph+=d)
//   {
//      glBegin(GL_QUAD_STRIP);
//      for (int th=0;th<=360;th+=d)
//      {
//         Vertex(th,ph);
//         Vertex(th,ph+d);
//      }
//      glEnd();
//   }
//
//   //  Undo transformations
//   glPopMatrix();
//}

void lake(GLfloat xl, GLfloat yl,GLfloat z,GLfloat radius)
                 
{
 GLfloat x              = xl;
    GLfloat y              = yl;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[11]);

    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(cos(angle),0, sin(angle));
            glVertex3f(x+xl, z,(y+yl));
            angle = angle + angle_stepsize;
        }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


static void house1(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double k1, double k2, double k3, int i)
{  
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
   glColor3f(k1,k2,k3);
   glBegin(GL_QUADS);
   //  Front
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

  //glBindTexture(GL_TEXTURE_2D, texture[i]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[i]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void house2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double k1, double k2, double k3, int i)
{  
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
   glColor3f(k1,k2,k3);
   if(ntex) glBindTexture(GL_TEXTURE_2D,texture[i]);
   glBegin(GL_QUADS);
   //  Front
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
static void house3(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double k1, double k2, double k3, int i)
{  
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
   glColor3f(k1,k2,k3);
   if(ntex) glBindTexture(GL_TEXTURE_2D,texture[i]);
   glBegin(GL_QUADS);
   //  Front
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
  glBindTexture(GL_TEXTURE_2D, texture[i]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
static void ground(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,-1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glColor3f(0.5,0.5,0.5);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   
   glColor3f(0.5,0.5,0.5);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(0.5,0.5,0.5);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(0.5,0.5,0.5);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(0.5,0.5,0.5);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[35]);

   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(0.5,0.5,0.5);
glBindTexture(GL_TEXTURE_2D,texture[5]);

   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


//static void grass(double x,double y,double z,
//                 double dx,double dy,double dz,
//                 double th)
//{
//    //  Set specular color to white
//   float white[] = {1,1,1,1};
//   float Emission[] = {0.0,0.0,0.001*emission,1.0};
//   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
//   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
//   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
//   //  Save transformation
//   glPushMatrix();
//   //  Offset
//   glTranslated(x,y,z);
//   glRotated(th,0,-1,0);
//   glScaled(dx,dy,dz);
//   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
//   glColor3f(1,1,1);
//   glBindTexture(GL_TEXTURE_2D,texture[5]);
//   glColor3f(0.5,0.5,0.5);
//   //  Cube
//   glBegin(GL_QUADS);
//   //  Front
//   glNormal3f(0,0,1);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,-1, 1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,-1, 1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,+1, 1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,+1, 1);
//   glEnd();
//   //  Back
//  
//   glColor3f(0.5,0.5,0.5);
//   glBegin(GL_QUADS);
//   glNormal3f(0,0,-1);
//   glTexCoord2f(0,0);
//   glVertex3f(+1,-1,-1);
//   glTexCoord2f(1,0);
//   glVertex3f(-1,-1,-1);
//   glTexCoord2f(1,1);
//   glVertex3f(-1,+1,-1);
//   glTexCoord2f(0,1);
//   glVertex3f(+1,+1,-1);
//   glEnd();
//   //  Right
//   glColor3f(0.5,0.5,0.5);
//   glBegin(GL_QUADS);
//   glNormal3f(1,0,0);
//   glTexCoord2f(0,0);
//   glVertex3f(+1,-1,+1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,-1,-1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,+1,-1);
//   glTexCoord2f(0,1);
//   glVertex3f(+1,+1,+1);
//   glEnd();
//   //  Left
//   glColor3f(0.5,0.5,0.5);
//   glBegin(GL_QUADS);
//   glNormal3f(-1,0,0);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,-1,-1);
//   glTexCoord2f(1,0);
//   glVertex3f(-1,-1,+1);
//   glTexCoord2f(1,1);
//   glVertex3f(-1,+1,+1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,+1,-1);
//   glEnd();
//   //  Top
//   glColor3f(0.5,0.5,0.5);
//   glBegin(GL_QUADS);
//   glNormal3f(0,1,0);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,+1,+1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,+1,+1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,+1,-1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,+1,-1);
//   glEnd();
//   //  Bottom
//   glColor3f(0.5,0.5,0.5);
//   glBegin(GL_QUADS);
//   glNormal3f(0,-1,0);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,-1,-1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,-1,-1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,-1,+1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,-1,+1);
//   //  End
//   glEnd();
//   //  Undo transformations
//   glPopMatrix();
//   glDisable(GL_TEXTURE_2D);
//}
static void blank1(double x,double y,double z,
                 double dx,double dy,double dz,double k1,double k2,
                 double k3, double th,int i)
{
     float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(k1,k2,k3);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
  // glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void blank2(double x,double y,double z,
                 double dx,double dy,double dz,double k1,double k2, double k3,
                 double th,int i)
{
     float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(k1,k2,k3);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
  // glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

void triangle(double x,double y,double z,
                 double dx,double dy,double dz,double th,double k1,double k2,double k3,int i)
{
 float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
  
glBegin(GL_TRIANGLES);
glColor3f(k1,k2,k3);
glNormal3f(0,0,1);
glTexCoord2f(0,0);
glVertex3f(x,y,z);
glTexCoord2f(1.0f, 0.0f); 
glVertex3f(x+dx,y,z);
glTexCoord2f(0.5f, 1.0f);
glVertex3f(x+dx/2, y+dy, z);
glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);}
static void draw_sphere(double x, double y, double z,double r, double x1, double y1){
//if((ccz<=9) && (ccx==0)){
//ccz = ccz + 0.01/2;
//}
//if(ccz>=9){
//ccz=-1;
//}


if((ccz<=9.5)){
ccz = ccz + 0.01/2;
//printf("%f",cz);
}
if(ccz>=9.5){
ccz=-1;
kk=0;
}


//if((ccz<=5.5) && (ccx==0)){
//ccz = ccz + 0.01;
//}
//else if((ccz>=5.0) && (ccx>-4.0)){
//ccx=ccx-0.01;
//}
//else if((ccx<0) && (ccz>=1.5)){
//ccz=ccz-0.01;}
//else if(ccz<=2 && ccx<=0){
//ccx=ccx+0.01;
//}
//else if(ccx>=0 && ccz<=2){
//ccx=0;}

glPushMatrix();

glTranslated(x+ccx,y,z+ccz);
glRotated(th,0,1,0);
glColor3f(0,0,0);
glutSolidSphere(r,x1,y1);
glPopMatrix();
}
static void draw_sphere2(double x, double y, double z,double r, double x1, double y1){
//if((ccz<=9) && (ccx==0)){
//ccz = ccz + 0.01/2;
//}
//if(ccz>=9){
//ccz=-1;
//}


if((cccz<=9.5) && (ccx==0)){
cccz = cccz + 0.01/2;
//printf("%f",cz);
}
if(cccz>=9.5){
cccz=-1;
}


//if((ccz<=5.5) && (ccx==0)){
//ccz = ccz + 0.01;
//}
//else if((ccz>=5.0) && (ccx>-4.0)){
//ccx=ccx-0.01;
//}
//else if((ccx<0) && (ccz>=1.5)){
//ccz=ccz-0.01;}
//else if(ccz<=2 && ccx<=0){
//ccx=ccx+0.01;
//}
//else if(ccx>=0 && ccz<=2){
//ccx=0;}

glPushMatrix();

glTranslated(x+ccx,y,z+cccz);
glRotated(th,0,1,0);
glColor3f(0,0,0);
glutSolidSphere(r,x1,y1);
glPopMatrix();
}
static void draw_cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,double k1,double k2,double k3,int i)
{
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(k1,k2,k3);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
  // glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
glBindTexture(GL_TEXTURE_2D,texture[i]);

   //if(ntex) glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
static void draw_car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,double k1,double k2,double k3,int i,int p)
{
   //  Save transformation
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
  

if((cz<=9.5)){
cz = cz + 0.01;
//printf("%f",cz);
}

if(cz>=9.5){
cz=-1;
k=0;
}



//else if((cz>=5.0) && (cx>-4.0)){
//cx=cx-0.01*4;
//k=2;
//}
//else if((cx<0) && (cz>=1.5)){
//  th=90;
//cz=cz-0.01*4;
//k=3;
//}
//else if(cz<=2 && cx<=0){
//  th=0;
//cx=cx+0.01*4;
//}
//else if(cx>=0 && cz<=2){
//cx=0;}


   //  Offset
   glTranslated(x+cx,y,z+cz);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
 
   //  Front
  glBegin(GL_QUADS);
   //  Front
   glColor3f(k1,k2,k3);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
  //glColor3f(0,1,1);
   //glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //glBindTexture(GL_TEXTURE_2D,texture[]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   if(p==0){
   glBindTexture(GL_TEXTURE_2D,texture[14]);}
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
   
   glBindTexture(GL_TEXTURE_2D, texture[i]);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

   
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
glBindTexture(GL_TEXTURE_2D,texture[i]);

   //glColor3f(0,1,1);
   //glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}
static void draw_car2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,double k1,double k2,double k3,int i,int p)
{
   //  Save transformation
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
  

if((czz<=9.5) && (cx==0)){
czz = czz + 0.01;
//printf("%f",cz);
}
if(czz>=9.5){
czz=-1;
}



//else if((cz>=5.0) && (cx>-4.0)){
//cx=cx-0.01*4;
//k=2;
//}
//else if((cx<0) && (cz>=1.5)){
//  th=90;
//cz=cz-0.01*4;
//k=3;
//}
//else if(cz<=2 && cx<=0){
//  th=0;
//cx=cx+0.01*4;
//}
//else if(cx>=0 && cz<=2){
//cx=0;}


   //  Offset
   glTranslated(x+cx,y,z+czz);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
 
   //  Front
  glBegin(GL_QUADS);
   //  Front
   glColor3f(k1,k2,k3);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
  // glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,1);
   //if(ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   if(p==0){
   glBindTexture(GL_TEXTURE_2D,texture[14]);}
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[i]);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(0,1,1);
  glBindTexture(GL_TEXTURE_2D, texture[i]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void plane(double x,double y,double z,
                 double dx,double dy,double dz,double th,double k1,double k2,double k3,int i)
{
float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[i]);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[33]);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glBegin(GL_QUADS);
   //  Front
glColor3f(k1,k2,k3);
glTexCoord2f(0,0);
   glVertex3f(x,y,z);
   glTexCoord2f(1,0);
   glVertex3f(x+dx/2,y+dy,z);
   glTexCoord2f(1,1);
   glVertex3f(x+dx/2,y+dy,z-dz);
   glTexCoord2f(0,1);
glVertex3f(x,y,z-dz);
 glEnd();
   //  Undo transformations
  glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void road(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[] = {0.0,0.0,0.001*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   
   glColor3f(1,1,1);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(1,1,1);
glBindTexture(GL_TEXTURE_2D,texture[7]);
if(sno) glBindTexture(GL_TEXTURE_2D,texture[34]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0);
   glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1);
   glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1);
   glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,1,1);
glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0);
   glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1);
   glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1);
   glVertex3f(-1,-1,+1);
   //  End
glEnd();

glPopMatrix();
glDisable(GL_TEXTURE_2D);
}
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = (float)(rand()%100)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 10)- 4.5 ;
    par_sys[i].ypos = 5;
    par_sys[i].zpos = (float) (rand() % 10) + 36;

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;//-0.8;

}
//static void pave(double x,double y,double z,
//                 double dx,double dy,double dz,
//                 double th)
//{
//    //  Set specular color to white
//   float white[] = {1,1,1,1};
//   float Emission[] = {0.0,0.0,0.001*emission,1.0};
//   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
//   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
//   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
//   //  Save transformation
//   glPushMatrix();
//   //  Offset
//   glTranslated(x,y,z);
//   glRotated(th,0,1,0);
//   glScaled(dx,dy,dz);
//  
//   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,pro?GL_REPLACE:GL_MODULATE);
//   glColor3f(1,1,1);
//   glBindTexture(GL_TEXTURE_2D,texture[10]);
//  
//   glColor3f(1,1,1);
//   //  Cube
//   glBegin(GL_QUADS);
//   //  Front
//  
//   glNormal3f(0,0,1);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,-1, 1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,-1, 1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,+1, 1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,+1, 1);
//   glEnd();
//   //  Back
//   glColor3f(1,1,1);
//   glBegin(GL_QUADS);
//   glNormal3f(0,0,-1);
//   glTexCoord2f(0,0);
//   glVertex3f(+1,-1,-1);
//   glTexCoord2f(1,0);
//   glVertex3f(-1,-1,-1);
//   glTexCoord2f(1,1);
//   glVertex3f(-1,+1,-1);
//   glTexCoord2f(0,1);
//   glVertex3f(+1,+1,-1);
//   glEnd();
//   //  Right
//   glColor3f(1,1,1);
//   glBegin(GL_QUADS);
//   glNormal3f(1,0,0);
//   glTexCoord2f(0,0);
//   glVertex3f(+1,-1,+1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,-1,-1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,+1,-1);
//   glTexCoord2f(0,1);
//   glVertex3f(+1,+1,+1);
//   glEnd();
//   //  Left
//   glColor3f(1,1,1);
//   glBegin(GL_QUADS);
//   glNormal3f(-1,0,0);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,-1,-1);
//   glTexCoord2f(1,0);
//   glVertex3f(-1,-1,+1);
//   glTexCoord2f(1,1);
//   glVertex3f(-1,+1,+1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,+1,-1);
//   glEnd();
//   //  Top
//   glColor3f(1,1,1);
//   glBegin(GL_QUADS);
//   glNormal3f(0,1,0);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,+1,+1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,+1,+1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,+1,-1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,+1,-1);
//   glEnd();
//   //  Bottom
//   glColor3f(1,1,1);
//   glBegin(GL_QUADS);
//   glNormal3f(0,-1,0);
//   glTexCoord2f(0,0);
//   glVertex3f(-1,-1,-1);
//   glTexCoord2f(1,0);
//   glVertex3f(+1,-1,-1);
//   glTexCoord2f(1,1);
//   glVertex3f(+1,-1,+1);
//   glTexCoord2f(0,1);
//   glVertex3f(-1,-1,+1);
//   //  End
//glEnd();
//
//glPopMatrix();
//glDisable(GL_TEXTURE_2D);
//}
//
/*
 *  Draw vertex in polar coordinates
 */

//static void ball(double x,double y,double z,double r)
//{
//   //  Save transformation
//   glPushMatrix();
//   //  Offset, scale and rotate
//   glTranslated(x,y,z);
//   glScaled(r,r,r);
//   //  White ball with yellow specular
//   float yellow[]   = {1.0,1.0,0.0,1.0};
//   float Emission[] = {0.0,0.0,0.01*emission,1.0};
//   glColor3f(1,1,1);
//   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
//   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
//   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
//   //  Bands of latitude
//   for (int ph=-90;ph<90;ph+=inc)
//   {
//      glBegin(GL_QUAD_STRIP);
//      for (int th=0;th<=360;th+=2*inc)
//      {
//         Vertex(th,ph);
//         Vertex(th,ph+inc);
//      }
//      glEnd();
//   }
//   //  Undo transofrmations
//   glPopMatrix();
//}
//
//static void sun(double x,double y,double z,double r)
//{
//   const int d=15;
//
//   //  Save transformation
//   glPushMatrix();
//   //  Offset and scale
//   glTranslated(x,y,z);
//   glScaled(r,r,r);
//   glEnable(GL_TEXTURE_2D);
//   glBindTexture(GL_TEXTURE_2D,texture[8]);
//   glColor3f(1.0,0.5,0.0);
//
//   //  Latitude bands
//   for (int ph=-90;ph<90;ph+=d)
//   {
//      glBegin(GL_QUAD_STRIP);
//      for (int th=0;th<=360;th+=d)
//      {
//         Vertex(th,ph);
//         Vertex(th,ph+d);
//      }
//      glEnd();
//   }
//
//   //  Undo transformations
//   glPopMatrix();
//   glDisable(GL_TEXTURE_2D);
//}

void drawSnow() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+1) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix();
      glTranslatef(x, y, z);
      glutSolidSphere(0.1/2, 16, 16);
      glPopMatrix();

      // Update values
      //Move
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= 0) {
sno=1;
// int zi = z - zoom + 10;
//        int xi = x + 10;
//        ground_colors[zi][xi][0] = 1.0;
//        ground_colors[zi][xi][2] = 1.0;
//        ground_colors[zi][xi][3] += 1.0;
//        if (ground_colors[zi][xi][3] > 1.0) {
//          ground_points[xi][zi][1] += 0.1;
//      }
}

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

// For Hail
void drawHail() {
  float x, y, z;

  for (loop = 0; loop < MAX_PARTICLES; loop=loop+1) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.8, 0.8, 0.9);
      glBegin(GL_QUADS);
        // Front
        glVertex3f(x-hailsize, y-hailsize, z+hailsize); // lower left
        glVertex3f(x-hailsize, y+hailsize, z+hailsize); // upper left
        glVertex3f(x+hailsize, y+hailsize, z+hailsize); // upper right
        glVertex3f(x+hailsize, y-hailsize, z+hailsize); // lower left
        //Left
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        // Back
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        //Right
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
        //Top
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        //Bottom
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
      glEnd();

      // Update values
      //Move
      if (par_sys[loop].ypos <= 0) {
        par_sys[loop].vel = par_sys[loop].vel * -1.0;
      }
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000); // * 1000
      par_sys[loop].vel += par_sys[loop].gravity;

      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}
// For Rain
void drawRain() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+1) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
//y=5;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(-x, y, z);
        glVertex3f(-x, y-0.1, z);
      glEnd();

      // Update values
      //Move
      // Adjust slowdown for speed!
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= 0) {
        par_sys[loop].life = -1.0;
      }
      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}
void init( ) {
  int x, z;

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

  //  Verticies
    // Ground Colors
    for (z = 0; z < 21; z++) {
      for (x = 0; x < 21; x++) {
        ground_points[x][z][0] = x - 10.0;
        ground_points[x][z][1] = accum;
        ground_points[x][z][2] = z - 10.0;

        ground_colors[z][x][0] = r; // red value
        ground_colors[z][x][1] = g; // green value
        ground_colors[z][x][2] = b; // blue value
        ground_colors[z][x][3] = 0.0; // acummulation factor
      }
    }

    // Initialize particles
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
    }
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
static void movingcar(){
//car
//draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5+0.3,1.45-0.5+2-5.5-0.5-0.5,0.5-0.2,0.2,0.3-0.15,90,0,1,0,29);
draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.5-0.1,0.2,0.3-0.1,90,0,0,1,29,0);
//draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.3,16,16);
draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5-0.1,1.45-0.5+2-5.5-0.5-0.5+0.5,0.3-0.1,0.1,0.3-0.1,90,0,1,0,29,1);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);

glRotated(180,0,1,0);
//draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5+0.3,1.45-0.5+2-5.5-0.5-0.5,0.5-0.2,0.2,0.3-0.15,90,0,1,0,29);
draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.5-0.1,0.2,0.3-0.1,90,0,0,1,32,0);
//draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.3,16,16);
draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5-0.1,1.45-0.5+2-5.5-0.5-0.5+0.5,0.3-0.1,0.1,0.3-0.1,90,0,1,0,32,1);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);
draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);

glRotated(90,0,1,0);
//draw_car( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5+0.3,1.45-0.5+2-5.5-0.5-0.5,0.5-0.2,0.2,0.3-0.15,90,0,1,0,29);
draw_car2( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.5-0.1,0.2,0.3-0.1,90,0,0,1,32,0);
//draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.3,16,16);
draw_car2( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5-0.1,1.45-0.5+2-5.5-0.5-0.5+0.5,0.3-0.1,0.1,0.3-0.1,90,0,1,0,32,1);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);

glRotated(-180,0,1,0);
draw_car2( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.5-0.1,0.2,0.3-0.1,90,0,0,1,29,0);
//draw_sphere(0.45-0.45-0.55-0.45-0.5+0.5+3,0.5,1.45-0.5+2-5.5-0.5-0.5,0.3,16,16);
draw_car2( 0.45-0.45-0.55-0.45-0.5+0.5+3,0.5-0.1,1.45-0.5+2-5.5-0.5-0.5+0.5,0.3-0.1,0.1,0.3-0.1,90,0,1,0,29,1);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5-0.2,0.1,16,16);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3-0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);
draw_sphere2(0.45-0.45-0.55-0.45-0.5+0.5+3+0.2,0.5-0.3,1.45-0.5+2-5.5-0.5-0.5+0.4,0.1,16,16);

}
void tree(){

trunk2(-3.5+1,0.5+3,0.05,0.1,1.05);
glTranslatef(-3.5+1,0.5+3,0.05);
glutSolidSphere(0.5,8,8);}
void display()
{


     //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
//   glClearColor(0.196078,0.6,0.8,1);
   //  Undo previous transformations
   glLoadIdentity();
   if(mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else if (mode == 0)
   {
     glRotatef(ph,1,0,0);
     glRotatef(th,0,1,0);
   }
 if (fall == RAIN) {
    drawRain();
  }else if (fall == HAIL) {
    drawHail();
  }else if (fall == SNOW) {
    drawSnow();
  }
//ground
ground( 0.0,0.05,0.0,4.5,0.05,4.5,90);  
//roads
road(0,0.06,2,4.5,0.05,0.35,0);
road(0,0.06,-2,4.5,0.05,0.35,0);
road(2,0.06,0,4.5,0.05,0.35,90);
road(-2,0.06,0,4.5,0.05,0.35,90);
//bottom row buildings
//buildings in 2nd row
house1( 1,1.05,4,0.3,1,0.3,0,1,0,0,0);
blank1(-1.17+2,0.25,2.6+1.2,0.1,0.2,0.15,0,0,0,0,30);
blank1(-1.125+2,1.0,2.55+1.2,0.05,0.2,0.10,0,0,0,0,14);
house1( 0,1.05,4,0.3,1,0.3,0,1,0,0,0);
blank1(-1.17+1,0.25,2.6+1.2,0.1,0.2,0.15,0,0,0,0,30);
blank1(-1.125+1,1.0,2.55+1.2,0.05,0.2,0.10,0,0,0,0,14);
house1( -1,1.05,4,0.3,1,0.3,0,1,0,0,0);
blank1(-1.17,0.25,2.6+1.2,0.1,0.2,0.15,0,0,0,0,30);
blank1(-1.125,1.0,2.55+1.2,0.05,0.2,0.10,0,0,0,0,14);
//buildings in 1st row
house1( 1,1.05,2.8,0.3,1,0.3,0,1,0,0,0);
blank1(-1.17+2,0.25,2.6,0.1,0.2,0.15,0,0,0,0,30);
blank1(-1.125+2,1.0,2.55,0.05,0.2,0.10,0,0,0,0,14);
house1( 0,1.05,2.8,0.3,1,0.3,0,1,0,0,0);
blank1(-1.17+1,0.25,2.6,0.1,0.2,0.15,0,0,0,0,30);
blank1(-1.125+1,1.0,2.55,0.05,0.2,0.10,0,0,0,0,14);
house1( -1,1.05,2.8,0.3,1,0.3,0,1,0,0,0);
blank1(-1.17,0.25,2.6,0.1,0.2,0.15,0,0,0,0,30);
blank1(-1.125,1.0,2.55,0.05,0.2,0.10,0,0,0,0,14);
//Middle row building
house2( -3.5,0.75,0,0.8,0.7,0.9,0,1,0,0,31);
blank1(-2.75 ,0.35 ,0 ,0.1,0.35,.32,0,0,0,0,30);
blank1(-2.75 ,0.35 ,0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3 ,0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35 ,0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3 ,0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
//Top row buildings
house3(-3.5,0.85,-3.5,0.8,0.8,0.8,0,1,0,0,22);
blank1(-3.5 ,0.35 ,-2.75 ,0.1,0.35,.32,0,0,0,90,24);
blank1(-3.5+0.6 ,0.35 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5+0.6 ,0.35+0.3 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5+0.6 ,0.35+0.6 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5+0.3 ,0.35+0.6 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5 ,0.35+0.6 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5-0.3 ,0.35+0.6 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5-0.6 ,0.35+0.6 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5-0.6 ,0.35+0.3 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-3.5-0.6 ,0.35 ,-2.75 ,0.1,0.1,0.1,0,0,0,0,14);
//windows for sides
//left
blank1(-2.75 ,0.35 ,-3.5+0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3 ,-3.5+0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,-3.5+0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,-3.5+0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,-3.5+0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,-3.5+0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3,-3.5+0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3,-3.5+0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3,-3.5+0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35,-3.5+0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35,-3.5+0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35,-3.5+0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35 ,-3.5+0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.3 ,-3.5+0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-2.75 ,0.35+0.6,-3.5+0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
//right
blank1(-4.35+0.1 ,0.35 ,-3.5+0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35+0.1 ,0.35+0.3 ,-3.5+0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35+0.6,-3.5+0+0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35+0.6,-3.5+0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35+0.6,-3.5+0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35+0.1 ,0.35+0.6,-3.5+0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35+0.3,-3.5+0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35+0.1 ,0.35+0.3,-3.5+0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35+0.1 ,0.35+0.3,-3.5+0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35,-3.5+0+0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35+0.1 ,0.35,-3.5+0 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35+0.1 ,0.35,-3.5+0-0.3 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35 ,-3.5+0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35+0.3 ,-3.5+0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);
blank1(-4.35 +0.1,0.35+0.6,-3.5+0-0.6 ,0.1,0.1,0.1,0,0,0,0,14);


//  glRotatef(pan, 0.0, 1.0, 0.0);
//  glRotatef(tilt, 1.0, 0.0, 0.0);

if(fog ==1)
    {
         GLfloat fogcolour[4]= {1.0,1.0,1.0,1.0};

        glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
                          /* How dense */
        glFogi(GL_FOG_MODE,GL_EXP);
        glFogf(GL_FOG_DENSITY,0.08);                   /* exponential decay */
        glFogf(GL_FOG_START,3.0);                   /* Where wwe start fogging */
        glFogf(GL_FOG_END,100.0);                       /* end */
        glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
        glEnable(GL_FOG);/* ENABLE */

    }
    else if(fog==0)
    {

        glDisable(GL_FOG);
    }


//church
draw_cube( 0.45-0.45,1.05,0.45-0.5,1,1,1,0,1,0,0,12);
blank1(0.45-0.45,0.8,0.45-0.5+1,0.3,0.8,0.1/2,0,0,0,0,13);
blank1(0.45-0.45,0.8+0.3,0.45-0.5+1-2,0.3,0.8,0.1/2,0,0,0,0,21);
blank1(0.45-0.45+0.6,1+0.3,0.45-0.5+1,0.2,0.4,0.1/2,0,0,0,0,15);
blank1(0.45-0.45-0.6,1+0.3,0.45-0.5+1,0.2,0.4,0.1/2,0,0,0,0,15);
plane(-0.55-0.45,2.05,1.45-0.5,2,2,2,0,0,1,1,17);
plane(-0.55+2-0.45,2.05,1.45-0.5,-2,2,2,0,0,1,1,17);
triangle(-0.55-0.45,2.05,1.45-0.5,2,2,2,0,0,1,0,12);
triangle(-0.55-0.45,2.05,-0.51-0.5,2,2,2,0,0,1,0,12);
blank2( -0.55-0.45+1,4.5,0.45-0.5+0.9 ,0.01*2,0.5,0.01*2,0,0,0,0,16);
blank2( -0.55-0.45+1,4.5+0.2,0.45-0.5+0.9 ,0.5,0.01*2,0.01*2,0,0,0,0,16);
//house
//left
draw_cube( 0.45-0.45-0.55-0.45-0.5+0.5,0.6,1.45-0.5+2-5.5-0.5-0.5,0.5,0.5,0.5,90,1,1,0,26);
plane(-0.55-0.45-0.5,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,0,1,1,19);
plane(-0.55-0.45-0.5+1,1.1,-0.51-0.5+2-4.04,-1,1,1,90,0,1,1,19);
triangle(-0.55-0.45-0.5,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,0,1,0,26);
triangle(-0.55-0.45-0.5,1.1,-0.51-0.01-0.5+2-5,1,1,1,90,0,1,0,26);
blank1(0.45-0.45-0.55-0.45-0.5+0.5 ,0.35 ,-2.75-0.36 ,0.1,0.35,.2,0,0,0,90,24);

 //right
draw_cube( 0.45-0.45-0.55-0.45-0.5+0.5+2,0.6,1.45-0.5+2-5.5-0.5-0.5,0.5,0.5,0.5,90,1,0,1,26);
plane(-0.55-0.45-0.5+2,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,0,1,1,19);
plane(-0.55-0.45-0.5+1+2,1.1,-0.51-0.5+2-4.04,-1,1,1,90,0,1,1,19);
triangle(-0.55-0.45+1.5,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,0,1,0,26);
triangle(-0.55-0.45+1.5,1.1,-0.51-0.01-0.5+2-5,1,1,1,90,0,1,0,26);
blank1(0.45-0.45-0.55-0.45-0.5+0.5 +2,0.35 ,-2.75-0.36 ,0.1,0.35,.2,0,0,0,90,24);


//tree left
tree1(-3.5,0.5+3,0.55,0.7,1.0);
//tree1(-3.5,0.5+3,0.65,0.68,0.8);
//tree1(-3.5,0.5+3,0.75,0.66,0.8);
tree2(-3.5,0.5+3,0.85,0.64,0.8);
//tree2 branch
trunk2(-3.5,0.5+3,0.05,0.1,1.05);
//tree right
tree1(3.5,0.5+3,0.55,0.7,1.0);
//tree1(3.5,0.5+3,0.65,0.68,0.8);
//tree1(3.5,0.5+3,0.75,0.66,0.8);
tree2(3.5,0.5+3,0.85,0.64,0.8);
//tree2 branch
trunk2(3.5,0.5+3,0.05,0.1,1.05);
//tree top
tree1(3.5,0.5-4,0.55,0.7,1.0);
//tree1(3.5,0.5-4,0.65,0.68,0.8);
//tree1(3.5,0.5-4,0.75,0.66,0.8);
tree2(3.5,0.5-4,0.85,0.64,0.8);
//tree2 branch
trunk2(3.5,0.5-4,0.05,0.1,1.05);




glRotated(-90,0,1,0);

//house
//left
draw_cube( 0.45-0.45-0.55-0.45-0.5+0.5,0.6,1.45-0.5+2-5.5-0.5-0.5,0.5,0.5,0.5,90,0,1,0,28);
plane(-0.55-0.45-0.5,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,1,1,0,18);
plane(-0.55-0.45-0.5+1,1.1,-0.51-0.5+2-4.04,-1,1,1,90,1,1,0,18);
triangle(-0.55-0.45-0.5,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,0,0,1,28);
triangle(-0.55-0.45-0.5,1.1,-0.51-0.01-0.5+2-5,1,1,1,90,0,0,1,28);
blank1(0.45-0.45-0.55-0.45-0.5+0.5 ,0.35 ,-2.75-0.36 ,0.1,0.35,.2,0,0,0,90,24);

 //right
draw_cube( 0.45-0.45-0.55-0.45-0.5+0.5+2,0.6,1.45-0.5+2-5.5-0.5-0.5,0.5,0.5,0.5,90,1,0,0,28);
plane(-0.55-0.45-0.5+2,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,1,1,0,18);
plane(-0.55-0.45-0.5+1+2,1.1,-0.51-0.5+2-4.04,-1,1,1,90,1,1,0,18);
triangle(-0.55-0.45+1.5,1.1,1.45-0.5+2-5.5-0.5,1,1,1,90,0,0,1,28);
triangle(-0.55-0.45+1.5,1.1,-0.51-0.01-0.5+2-5,1,1,1,90,0,0,1,28);
blank1(0.45-0.45-0.55-0.45-0.5+0.5 +2,0.35 ,-2.75-0.36 ,0.1,0.35,.2,0,0,0,90,24);

 glColor3f(1,1,1);


   //  Draw axes
   if (axes)
   {
      const double len=1.5;  //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
movingcar();
glRotated(zh,0,1,0);
   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

//   cube(1,0.01,1 , -1.5,0.05,1, 0);
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Ex=%f Ey=1.00 Ez=%f dx=%f dy=%f dz=%f angle=%f,mode=%d",th,ph,dim,fov,x1+lx,z1,lx,ly,lz,angle,mode);
if(mode==0)
Print("Projection = Orthogonal");
else if(mode==1)
Print("Projection = Perspective");
else
Print("projection = First Person view");  
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Elevation=%.1f",ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   
   //  Render the scene
   ErrCheck("display");
   glutSwapBuffers();

}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds

 double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode,fov,asp,dim,x1,z1,lx,ly,lz,s);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
//   Project(mode,fov,asp,dim,x,z,lx,ly,lz,s);
   //  Tell GLUT it is necessary to redisplay the scene
//   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
//   else if (ch=='s' && specular>0)
//      specular -= 5;
//   else if (ch=='S' && specular<100)
//      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
//   else if (ch=='+')
//      rep++;
//   else if (ch=='-' && rep>1)
//      rep--;
//   else if (ch == '-' )
//      fov--;
//   else if (ch == '+' )
//      fov++;
if (ch == 27)
      exit(0);
   //  Reset view angle
else if(ch=='w' )
{
s+=0.1;
s+=0.1;
s+=0.1;
}
if (ch == 'r') { // Rain
fog=0;
sno=0;
    fall = RAIN;
    glutPostRedisplay();
  }
  if (ch == 'h') { // Hail
fog=1;
sno=0;
    fall = HAIL;
    glutPostRedisplay();
  }
  if (ch == 'l') { // Snow
fog=1;
    fall = SNOW;
    glutPostRedisplay();
  }
  if (ch == '=') { //really '+' - make hail bigger
    hailsize += 0.01;
  }
  if (ch == '-') { // make hail smaller
    if (hailsize > 0.1) hailsize -= 0.01;
  }
  if (ch == ',') { // really '<' slow down
    if (slowdown > 4.0) slowdown += 0.01;
  }
  if (ch == '.') { // really '>' speed up
    if (slowdown > 1.0) slowdown -= 0.01;
  }
  if (ch == 'q') { // QUIT
    exit(0);
  }
else if(ch=='s' && s >= 0.1)
{
s-=0.1;
s-=0.1;
s-=0.1;
}
else if(ch=='a')
{
angle += 0.05;
    lx += sin(angle);
//  lz  -=cos(angle);
//lx+=0.1;
}
else if(ch=='d')
{
angle -= 0.05;
    lx += sin(angle);
//  lz  -=cos(angle);
//lx-=0.1;
}
else if (ch == '0')
      {
th = ph = 0;
x1=1.0f;
z1=1.0f;
lx=0.0;
ly=0.0;
lz=0.1;
angle=0;
s=1.0;
}
//  Toggle axes
   //  Switch display mode
   //else if (ch == 'm')
//mode=(mode+1)%2;
// else if (ch == 'M')
//      mode = (mode+3)%4;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode,fov,asp,dim,x1,z1,lx,ly,lz,s);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(mode,fov,asp,dim,x1,z1,lx,ly,lz,s);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Final Project- Jhansi Saketa BV and Tanmay Desai");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0]= LoadTexBMP("wall1.bmp");
   texture[1]= LoadTexBMP("wall2.bmp");
   texture[2]=LoadTexBMP("trunk1.bmp");
   texture[3]= LoadTexBMP("tree.bmp");
   texture[4]= LoadTexBMP("roof1.bmp");
   texture[5]= LoadTexBMP("grass.bmp");
   texture[6]= LoadTexBMP("ground.bmp");
   texture[7]=LoadTexBMP("road.bmp");
   texture[8]=LoadTexBMP("sun.bmp");
   texture[9]=LoadTexBMP("wall3.bmp");
   texture[10]=LoadTexBMP("pave.bmp");
   texture[11]=LoadTexBMP("water.bmp");
   texture[12]=LoadTexBMP("church.bmp");
   texture[13]=LoadTexBMP("churchdoor.bmp");
   texture[14]=LoadTexBMP("win1.bmp");
   texture[15]=LoadTexBMP("mothermary.bmp");
   texture[16]=LoadTexBMP("marble.bmp");
   texture[17]=LoadTexBMP("wood1.bmp");
   texture[18]=LoadTexBMP("wood2.bmp");
   texture[19]=LoadTexBMP("wood3.bmp");
   texture[20]=LoadTexBMP("wood4.bmp");
   texture[21]=LoadTexBMP("jesus.bmp");
   texture[22]=LoadTexBMP("tex1.bmp");
   texture[23]=LoadTexBMP("tex2.bmp");
   texture[24]=LoadTexBMP("door1.bmp");
   texture[25]=LoadTexBMP("door2.bmp");
   texture[26]=LoadTexBMP("tex5.bmp");
   texture[27]=LoadTexBMP("tex6.bmp");
   texture[28]=LoadTexBMP("tex7.bmp");
   texture[30]=LoadTexBMP("door4.bmp");
   texture[29]=LoadTexBMP("car1.bmp");
   texture[31]=LoadTexBMP("texture9.bmp");
   texture[32]=LoadTexBMP("car2.bmp");
texture[33]=LoadTexBMP("snow2.bmp");
texture[34]=LoadTexBMP("sno4.bmp");
texture[35]=LoadTexBMP("snow1.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
