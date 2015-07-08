/* main1.cpp */

#include <stdio.h>

#include <stdint.h>

#include <QApplication>
#include "CGLWin.hpp"

int main(int argc, char *argv[])
 {

  QApplication a(argc, argv);

  CGLWin glWin;
 
  if(!glWin.initFrames())
   {
    return(0);
   }

  glWin.resize(300,300);
  glWin.show();
  glWin.update();

  printf("Starting app\n");

  return a.exec();
 }
