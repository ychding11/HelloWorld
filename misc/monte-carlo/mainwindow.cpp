/****************************************************************************
**
**
****************************************************************************/

#include <Qt>
#include <QPainter>
#include <QKeyEvent>
#include <QStatusBar>
#include <math.h>
#include "mainwindow.h"

MacbethWidget::MacbethWidget(QWidget *parent) : QMainWindow(parent)
{
	macBethPic = new PictureDisplay();
	setCentralWidget(macBethPic);
	connect(&thread, SIGNAL(renderedImage(QImage,int)), this, SLOT(updatePicture(QImage,int)));
	thread.start();
	statusBar()->showMessage(tr("Rendering"));
}

void MacbethWidget::updatePicture(QImage image, int nPass)
{
//	printf("receive update signal.\n");
	char message[128];
	sprintf(message, "Received update signal. pass = %d", nPass);
	statusBar()->showMessage(tr(message));
	macBethPic->updateImage(image);
}

//void MacbethWidget::paintEvent(QPaintEvent * /* event */)
//{
//    QPainter painter(this);
//    painter.fillRect(rect(), Qt::black);
//
//    painter.setPen(Qt::white);
//    painter.drawText(rect(), Qt::AlignCenter, tr("Rendering initial image, please wait..."));
//    return;
//}

void MacbethWidget::resizeEvent(QResizeEvent * /* event */)
{
}

/*********************************************************
 * Function:
 *	
 *
 * Document Reference:
 *	http://doc.qt.io/qt-5/qkeyevent.html
 *
 ********************************************************/
void MacbethWidget::keyPressEvent(QKeyEvent *event)
{
    static bool paused = false;
    switch (event->key())
    {
    case Qt::Key_Escape:
    	close();
        break;
    case Qt::Key_Space:
    	(paused = !paused)? thread.pause() : thread.resume();
	printf("Press Space Key. paused = %d\n", paused);
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Up:
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void MacbethWidget::mousePressEvent(QMouseEvent *event)
{
}

void MacbethWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
    }
}

void MacbethWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
    }
}


/************** PictureDisplay Implementation *****************/

PictureDisplay::PictureDisplay(const string& filename)
{
  if (!pict.load(filename.c_str()))
  	throw runtime_error("Couldn't load image file " + filename);
  setMinimumSize(pict.width(), pict.height());
}

PictureDisplay::PictureDisplay()
{
}

void PictureDisplay::updateImage(const QImage &image)
{
//	pict = image;
	pict.load("./mcbeth.ppm");
	setMinimumSize(pict.width(), pict.height());
	update();
}

void PictureDisplay::paintEvent(QPaintEvent *)
{
  QPainter(this).drawImage(QPoint(0, 0), pict);
//  printf("repaint PictureDisplay.\n");
}

void PictureDisplay::mousePressEvent(QMouseEvent* event)
{
  emit clicked(event->pos());
}
