#ifndef GIFSAVER_H
#define GIFSAVER_H

#include <QString>
#include <QThread>

class Filter;
class Image;

class GifSaver : public QThread
{
    Q_OBJECT
public:
    GifSaver( const QString& outputFilePath,
              Filter* filter,
              float scaleFactor,
              const std::vector< Image* >& inputAnimatedGif );

    void save();

private:

    const QString _outputFilePath;
    Filter* _filter;
    float _scaleFactor;
    const std::vector< Image* >& _inputAnimatedGif;

    void run() Q_DECL_OVERRIDE;

signals:

    void setProgress( int percentage );
};

#endif // GIFSAVER_H
