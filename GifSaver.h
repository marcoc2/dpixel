#ifndef GIFSAVER_H
#define GIFSAVER_H

#include <QString>

class Filter;

class GifSaver
{
public:
    GifSaver();
    static void save( const QString& originalFilePath,
                      const QString& outputFilePath,
                      Filter* filter,
                      float scaleFactor  );
};

#endif // GIFSAVER_H
