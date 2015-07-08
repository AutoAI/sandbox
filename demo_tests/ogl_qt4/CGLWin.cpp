/**********************************************************************
CGLWin.cpp

**********************************************************************/

#include "CGLWin.hpp"

#include <stdio.h>

/*********************************************************************
cons()
*********************************************************************/

CGLWin::CGLWin(QWidget *p)
       : QGLWidget(p)
{
  mWidth = WIDTH;
  mHeight = HEIGHT;

  mpGrayData = NULL;
  mpRGBData = NULL;
}

/*********************************************************************
*********************************************************************/

void CGLWin::initializeGL()
{

  printf("intializeGL()\n");

  makeCurrent();
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  return;
}   // initializeGL()

/*********************************************************************
*********************************************************************/

void CGLWin::resizeGL(int w, int h)
{
  printf("resizeGL()\n");

  glViewport(0,0,mWidth,mHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0,mWidth,0,mHeight,-1,1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  return;
}   // resizeGL()

/*********************************************************************
*********************************************************************/

void CGLWin::paintGL()
{
  printf("paintGL()\n");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glLineWidth(3.0);
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f(0,0);
    glVertex2f(mWidth-1,0);
    glVertex2f(mWidth-1,mHeight-1);
    glVertex2f(0,mHeight-1);
  glEnd();

  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f(1,1);
    glVertex2f(mWidth/2, 1);
    glVertex2f(mWidth/2, mHeight/2);
    glVertex2f(1,mHeight/2);
  glEnd();

  glRasterPos2i(mWidth/2, mHeight/2);
  glDrawPixels(WIDTH/2, HEIGHT/2,
               GL_LUMINANCE,
               GL_UNSIGNED_BYTE,
               mpGrayData);

  glRasterPos2i(1,mHeight/2);
  glDrawPixels(WIDTH/2, HEIGHT/2,
               GL_RGBA,
               GL_UNSIGNED_INT_8_8_8_8,
               mpRGBData);
  glFlush();
  return;
}     // paintGL()

/*********************************************************************
*********************************************************************/

bool CGLWin::initFrames()
{

  uint32_t dX = mWidth / 2,
           dY = mHeight/2;

// OK if the data was already allocated;
//
  if(mpGrayData || mpRGBData)
   {
    return(true);
   }  
  mpGrayData = new uint8_t[dX * dY];
  if(!mpGrayData)
   return(false);

  mpRGBData = new uint32_t[dX * dY];
  if(!mpRGBData)
   return(false);

  int i = 0;
  for(int iY = 0; iY < dY; iY++)
   {
    for(int iX = 0; iX < dX; iX++)
     {
      i = (iY * dX) + iX;
      mpGrayData[ i ] = iX % 255;

      mpRGBData[i] = (iX / 255)
                     ? 0xFF0000FF
                     : 0x0000FFFF ;
     }
   }

  return(true);
}   // initFrames()
