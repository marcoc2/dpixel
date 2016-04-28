#include <QImageReader>

#include "Image.h"
#include "CheckUpscaleWindow.h"
#include "ui_CheckUpscaleWindow.h"
#include "MainWindow.h"

#define SCALE_FACTOR 10

CheckUpscaleWindow::CheckUpscaleWindow( QWidget *parent, Image* image ) :
    QDialog( parent ),
    _ui( new Ui::CheckUpscaleWindow ),
    _image( image ),
    _resizedImage( nullptr ),
    _imageScene( nullptr )
{
    _ui->setupUi( this );

    connect( _ui->okPushButton, SIGNAL( clicked() ), this, SLOT( okButtonCallback() ) );
    connect( _ui->downscaleSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( downscaleImage() ) );

    _imageScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( _image->getQImage()->scaled( _image->getQImage()->size() * SCALE_FACTOR ) );
    _imageScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _imageScene );

    show();
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
    QImage* scaledImage = new QImage ( _image->getQImage()->scaled(
                                      1.0f / ( float ) resizedFactor * _image->getQImage()->size() ) );

    if( _resizedImage )
    {
        delete _resizedImage;
    }

    _resizedImage = new Image( scaledImage );

    if( _imageScene != nullptr )
    {
        delete _imageScene;
    }

    _imageScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage(
                _resizedImage->getQImage()->scaled( _resizedImage->getQImage()->size() * SCALE_FACTOR * resizedFactor ) );
    _imageScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _imageScene );
}
