#ifndef FILTER_H
#define FILTER_H

#include <QThread>
#include "Image.h"

class Filter : public QThread
{
    Q_OBJECT
public:
    Filter( Image* inputImage, Image* outputImage );
    Filter( Image* inputImage, float scaleFactor );
    Filter( Image* inputImage, float scaleFactor, int numberOfPasses );
    Filter( Image* inputImage, int outputWidth, int outputHeight );
    Filter( Image* inputImage );
    ~Filter();

    virtual void apply() = 0;
    virtual void run() Q_DECL_OVERRIDE;

    Image* getOutputImage();
    int getScaleFactor();
    void setNewInputImage( Image* image );

protected:

    void fillBufferBGRA( u_char* inputBuffer );
    void fillImageBGRA( u_char* outputBuffer );

    void fillBufferRGB( u_char* inputBuffer );
    void fillImageRGB( u_char* outputBuffer );

    Image* _inputImage;
    Image* _outputImage;

    float _scaleFactor;

signals:

    void resultReady();
};

#endif // FILTER_H
