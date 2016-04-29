#include <QFileDialog>
#include <QImageReader>
#include "GifSaver.h"
#include "Image.h"
#include "Filters/Filter.h"
#include "gif.h"

GifSaver::GifSaver(  )
{

}


void GifSaver::save( const QString& originalFilePath,
                     const QString& outputFilePath,
                     Filter* filter,
                     float scaleFactor )
{
    QImageReader qImageReader( originalFilePath );

    std::vector< uint8_t* > bufferVector;
    GifWriter gifWriter;
    GifBegin( &gifWriter, outputFilePath.toStdString().c_str(),
              qImageReader.size().width() * scaleFactor,
              qImageReader.size().height() * scaleFactor,
              10 );

    for( int i = 0; i < qImageReader.imageCount(); i++ )
    {
        qImageReader.jumpToImage( i );

        QImage* qFrame = new QImage( qImageReader.read() );
        Image* originalFrame = new Image( qFrame );

        filter->setNewInputImage( originalFrame );
        filter->apply();

        Image* outputFrame = new Image( new QImage( *( filter->getOutputImage()->getQImage() ) ) );

        const int rgbaSize = 4;
        uint8_t* outputFrameBuffer = new uint8_t[ outputFrame->getWidth() * outputFrame->getHeight() * rgbaSize ];

        outputFrame->getBufferRGBA8( outputFrameBuffer );

        bufferVector.push_back( outputFrameBuffer );

        GifWriteFrame( &gifWriter,
                       outputFrameBuffer,
                       outputFrame->getWidth(),
                       outputFrame->getHeight(), 10 );

        delete originalFrame;
    }

    GifEnd( &gifWriter );

    for( auto const& buffer : bufferVector )
    {
        delete[] buffer;
    }
}

