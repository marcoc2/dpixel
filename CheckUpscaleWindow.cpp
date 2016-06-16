#include <QImageReader>
#include <QDesktopWidget>

#include "Image.h"
#include "CheckUpscaleWindow.h"
#include "ui_CheckUpscaleWindow.h"
#include "MainWindow.h"

#define SCALE_FACTOR 10

CheckUpscaleWindow::CheckUpscaleWindow( QWidget* parent, Image* image ) :
    QDialog( parent ),
    _ui( new Ui::CheckUpscaleWindow ),
    _copiedImage( image ),
    _resizedImage( nullptr ),
    _imageScene( nullptr )
{
    _ui->setupUi( this );

    connect( _ui->okPushButton, SIGNAL( clicked() ), this, SLOT( okButtonCallback() ) );
    connect( _ui->downscaleSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( downscaleImage() ) );

    QImage* scaledImage =
            new QImage( _copiedImage->getQImage()->scaled( SCALE_FACTOR * _copiedImage->getQImage()->size() ) );
    _resizedImage = new Image( scaledImage );
    drawGridOnImage( 1 );

    _imageScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( *( _resizedImage->getQImage() ) );
    _imageScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _imageScene );

    _fitGraphicsScene = new QGraphicsScene( this );
    _fitGraphicsScene->addPixmap( QPixmap::fromImage( QImage( "../Doc/fit.png" ) ) );
    _ui->fitGraphicsView->setScene( _fitGraphicsScene );

    setWindowFlags( Qt::Window | Qt::CustomizeWindowHint );
    //setWindowFlags(Qt::FramelessWindowHint);
    //setWindowFlags( Qt::X11BypassWindowManagerHint );

    //QRect screenGeometry = QApplication::desktop()->screenGeometry();
    //int x = ( screenGeometry.width() - this->width() ) / 2;
    //int y = ( screenGeometry.height() - this->height() ) / 2;
    //this->move( x, y );
    //this->show();

    show();
}


CheckUpscaleWindow::~CheckUpscaleWindow()
{
    delete _ui;
    delete _resizedImage;
    delete _copiedImage;
    delete _imageScene;
    delete _fitGraphicsScene;
}


void CheckUpscaleWindow::okButtonCallback()
{
    int downscaleFactor = _ui->downscaleSpinBox->value();
    if( downscaleFactor > 1 )
    {
        static_cast< MainWindow* >( parent() )->downscaleInputImage( downscaleFactor );
    }
    delete this;
}


void CheckUpscaleWindow::downscaleImage()
{
    int resizedFactor = _ui->downscaleSpinBox->value();
    QImage* scaledImage = new QImage( _copiedImage->getQImage()->scaled(
                                          1.0f / ( float ) resizedFactor * _copiedImage->getQImage()->size() ) );

    if( _resizedImage )
    {
        delete _resizedImage;
    }

    scaledImage = new QImage( scaledImage->scaled( scaledImage->size() * SCALE_FACTOR * resizedFactor ) );

    _resizedImage = new Image( scaledImage );

    drawGridOnImage( resizedFactor );

    if( _imageScene != nullptr )
    {
        delete _imageScene;
    }

    _imageScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( *( _resizedImage->getQImage() ) );
    _imageScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _imageScene );
}


void CheckUpscaleWindow::drawGridOnImage( int resizedFactor )
{
    for( size_t i = 0; i < _resizedImage->getWidth(); i = i + ( SCALE_FACTOR * resizedFactor ) )
    {
        for( size_t j = 0; j < _resizedImage->getHeight(); j++ )
        {
            _resizedImage->setPixel( i, j, Pixel( 0, 0, 0 ) );
        }
    }

    for( size_t j = 0; j < _resizedImage->getHeight(); j = j + SCALE_FACTOR * resizedFactor )
    {
        for( size_t i = 0; i < _resizedImage->getWidth(); i++ )
        {
            _resizedImage->setPixel( i, j, Pixel( 0, 0, 0 ) );
        }
    }

    _resizedImage->fillQImageRGB();
}


