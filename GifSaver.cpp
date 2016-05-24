#include <QFileDialog>
#include "GifSaver.h"
#include "Image.h"
#include "Filters/Filter.h"
#include "gif.h"

GifSaver::GifSaver( const QString& outputFilePath,
                    Filter* filter,
                    float scaleFactor,
                    const std::vector< Image* >& inputAnimatedGif ) :
    _outputFilePath( outputFilePath ),
    _filter( filter ),
    _scaleFactor( scaleFactor ),
    _inputAnimatedGif( inputAnimatedGif )
{

}


void GifSaver::run()
{
    save();
}


void GifSaver::save()
{
    std::vector< uint8_t* > bufferVector;
    GifWriter gifWriter;
    GifBegin( &gifWriter, _outputFilePath.toStdString().c_str(),
              _inputAnimatedGif[ 0 ]->getWidth() * _scaleFactor,
              _inputAnimatedGif[ 0 ]->getHeight() * _scaleFactor,
              10 );

    int i = 0;
    for( const auto& frame : _inputAnimatedGif )
    {
        _filter->setNewInputImage( frame );
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

        delete outputFrame;

        emit setProgress( ( float ) i++ * 100 / _inputAnimatedGif.size() );
    }

    GifEnd( &gifWriter );

    for( auto const& buffer : bufferVector )
    {
        delete[] buffer;
    }
}

