/*********************************************************************
*********************************************************************/

#if !defined(_CGLWIN_HPP)
#define _CGLWIN_HPP

#define WIDTH 640
#define HEIGHT 480

#include <QGLWidget>
#include <QWidget>

class CGLWin : public QGLWidget
{

  //  Q_OBJECT

public:

  CGLWin(QWidget *p = 0);

  bool initFrames();

protected:

  void initializeGL();

  void resizeGL(int w, int h);

  void paintGL();

private:

  uint32_t mWidth,
           mHeight;

  uint8_t *mpGrayData;

  uint32_t *mpRGBData;

};
#endif      // _CGLWIN_HPP

