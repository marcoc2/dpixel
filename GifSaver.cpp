#include <QFileDialog>
#include <QImageReader>
#include "GifSaver.h"
#include "Image.h"
#include "Filters/Filter.h"
#include "gif.h"

GifSaver::GifSaver( const QString& originalFilePath,
                    const QString& outputFilePath,
                    Filter* filter,
                    float scaleFactor ) :
    _originalFilePath( originalFilePath ),
    _outputFilePath( outputFilePath ),
    _filter( filter ),
    _scaleFactor( scaleFactor )
{

}


void GifSaver::run()
{
    save();
}


void GifSaver::save()
{
    QImageReader qImageReader( _originalFilePath );

    std::vector< uint8_t* > bufferVector;
    GifWriter gifWriter;
    GifBegin( &gifWriter, _outputFilePath.toStdString().c_str(),
              qImageReader.size().width() * _scaleFactor,
              qImageReader.size().height() * _scaleFactor,
              10 );

    for( int i = 0; i < qImageReader.imageCount(); i++ )
    {
        qImageReader.jumpToImage( i );

        QImage* qFrame = new QImage( qImageReader.read() );
        Image* originalFrame = new Image( qFrame );

        _filter->setNewInputImage( originalFrame );
        _filter->apply();

        Image* outputFrame = new Image( new QImage( *( _filter->getOutputImage()->getQImage() ) ) );

        const int rgbaSize = 4;
        uint8_t* outputFrameBuffer = new uint8_t[ outputFrame->getWidth() * outputFrame->getHeight() * rgbaSize ];

        outputFrame->getBufferRGBA8( outputFrameBuffer );

        bufferVector.push_back( outputFrameBuffer );

        GifWriteFrame( &gifWriter,
                       outputFrameBuffer,
                       outputFrame->getWidth(),
                       outputFrame->getHeight(), 10 );

        delete originalFrame;

        emit setProgress( ( float ) i * 100 / qImageReader.imageCount() );
    }

    GifEnd( &gifWriter );

    for( auto const& buffer : bufferVector )
    {
        delete[] buffer;
    }
}

