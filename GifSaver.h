#ifndef GIFSAVER_H
#define GIFSAVER_H

#include <QString>
#include <QThread>

class Filter;

class GifSaver : public QThread
{
    Q_OBJECT
public:
    GifSaver( const QString& originalFilePath,
              const QString& outputFilePath,
              Filter* filter,
              float scaleFactor );

    void save();

private:

    const QString _originalFilePath;
    const QString _outputFilePath;
    Filter* _filter;
    float _scaleFactor;

    void run() Q_DECL_OVERRIDE;

signals:

    void setProgress( int percentage );
};

#endif // GIFSAVER_H
