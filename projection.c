//  CSCIx229 library
//  Willem A. (Vlakkies) Schreuder
#include "CSCIx229.h"


//
//  Set projection
//
//void Project(double fov,double asp,double dim)
//{
//   //  Tell OpenGL we want to manipulate the projection matrix
//   glMatrixMode(GL_PROJECTION);
//   //  Undo previous transformations
//   glLoadIdentity();
//   //  Perspective transformation
//   if (fov)
//      gluPerspective(fov,asp,dim/16,16*dim);
//   //  Orthogonal transformation
//   else
//      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
//   //  Switch to manipulating the model matrix
//   glMatrixMode(GL_MODELVIEW);
//   //  Undo previous transformations
//   glLoadIdentity();
//}


void Project(double mode,double fov,double asp,double dim,double x,double z,double lx,double ly,double lz,double s, double fp)
{


   //  Tell OpenGL we want to manipulate the projection matrix
//   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
//   glLoadIdentity();
   //  Perspective transformation
//   if (mode==1)
//      gluPerspective(fov,asp,dim/16,16*dim);
   //  Orthogonal transformation
//   else
//      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
   //  Switch to manipulating the model matrix
//   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
//   glLoadIdentity();

//  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fp) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else {
      if (mode)
         gluPerspective(fov,asp,dim/4,4*dim);
      //  Orthogonal projection
      else
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   }  
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
   }




//   //  Tell OpenGL we want to manipulate the projection matrix
//   glMatrixMode(GL_PROJECTION);
//   //  Undo previous transformations
//   glLoadIdentity();
//   //  Perspective transformation
//  gluLookAt(	x, 1.0f, z,
//			x+lx, 1.0f,  z+lz,
//			0.0f, 1.0f,  0.0f);
//gluLookAt(0,0,3,0,0,0,0,1,0);
//lets try forward
//gluLookAt(0,0,3-,0,0,0-1,0,1,0);
//glScalef(s,s,s);
//gluLookAt(lx,1, z, x+lx, 1, z+lz, 0.0, 1.0, 0.0);
//
//  Switch to manipulating the model matrix
//   glMatrixMode(GL_MODELVIEW);
//   //  Undo previous transformations
//   glLoadIdentity();

