#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

class QImage;


class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(QObject *parent = 0);
    ~RenderThread();

    void render();

signals:
    void renderedImage(const QImage &image, int nPass);

protected:
    void run() Q_DECL_OVERRIDE;

private:
	int tester(void);
	void monteCarloIntegration(short curveIndex, float &X, float &Y, float &Z);

	int fNumPass = 0; // default initializer
	const float fGamma = 1.0 / 2.2;
	const int fSamples = 32;
	
};

#endif // RENDERTHREAD_H
