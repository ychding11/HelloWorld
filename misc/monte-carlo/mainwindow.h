/****************************************************************************
**
**
****************************************************************************/

#ifndef MANDELBROTWIDGET_H
#define MANDELBROTWIDGET_H

#include <QPixmap>
#include <QWidget>
#include <QMainWindow>
#include <string>
#include "renderthread.h"

using namespace std;

class PictureDisplay;

class MacbethWidget : public QMainWindow
{
    Q_OBJECT

public:
    MacbethWidget(QWidget *parent = 0);

protected:
    //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
	void updatePicture(QImage image, int nPass);
private:
    RenderThread thread;
    PictureDisplay* macBethPic;
};

//
//reference http://doc.qt.io/qt-5/qwidget.html#public-functions
//reference http://doc.qt.io/qt-5/qimage.html
class PictureDisplay : public QWidget
{
  Q_OBJECT

public:
  explicit PictureDisplay(const string& filename);
  explicit PictureDisplay();
  void updateImage(const QImage &image);
protected:
  void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent*)Q_DECL_OVERRIDE;
//  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
  
private:
  QImage pict;     // The image data 
  
signals:
  void clicked(const QPoint& where);  // Emitted on a click
};

#endif // MANDELBROTWIDGET_H
