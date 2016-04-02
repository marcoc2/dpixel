#include <stdio.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Widgets/RenderArea.h"

#include "SimilarityGraph.h"
#include "Diagram.h"
#include "Filters/hqx/HqxFilter.h"
#include "Filters/xBR/XbrFilter.h"
#include "Filters/xBRZ/XbrZFilter.h"
#include "Filters/Scale2xFilter.h"
#include "Filters/EagleFilter.h"
#include "Filters/CRTFilter.h"

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    _ui( new Ui::MainWindow ),
    _inputImage( nullptr ),
    _resultScene( nullptr ),
    _originalScene( nullptr ),
    _graphScene( nullptr )
{
    #ifdef _WIN32
        _inputImage = new Image( "../pixel-art-remaster/Samples/metalslug.png" );
    #else
        _inputImage = new Image( "../Samples/metalslug.png" );
    #endif


    _ui->setupUi( this );

    connect( _ui->testButton, SIGNAL( clicked() ), this, SLOT( createTest() ) );
    connect( _ui->loadImageButton, SIGNAL( clicked() ), this, SLOT( loadImage() ) );
    connect( _ui->saveImageButton, SIGNAL( clicked() ), this, SLOT( saveImage() ) );

    connect( _ui->radioButtonOriginal, SIGNAL( clicked() ), this, SLOT( loadOriginal() ) );
    connect( _ui->radioButtonNearest, SIGNAL( clicked() ), this, SLOT( applyNearest() ) );
    connect( _ui->radioButtonHqx, SIGNAL( clicked() ), this, SLOT( applyHqx() ) );
    connect( _ui->radioButtonXbr, SIGNAL( clicked() ), this, SLOT( applyXbr() ) );
    connect( _ui->radioButtonXbrZ, SIGNAL( clicked() ), this, SLOT( applyXbrZ() ) );
    connect( _ui->radioButtonScale2x, SIGNAL( clicked() ), this, SLOT( applyScale2x() ) );
    connect( _ui->radioButtonEagle, SIGNAL( clicked() ), this, SLOT( applyEagle() ) );
    connect( _ui->radioButtonCRT, SIGNAL( clicked() ), this, SLOT( applyCRT() ) );
    connect( _ui->radioButtonVector, SIGNAL( clicked() ), this, SLOT( applyVector() ) );

    connect( _ui->spinBoxNearestScaleFactor, SIGNAL( valueChanged( int ) ), this, SLOT( applyNearest()) );
    connect( _ui->spinBoxScaleFactor, SIGNAL( valueChanged( int ) ), this, SLOT( applyCRT() ) );

    enableFiltersFrame();

    createActions();
    createMenus();

    if( !_inputImage->getQImage()->isNull() )
    {
        fillQGraphicsView( *( _inputImage->getQImage() ), 1 );
        fillQGraphicsViewOriginal( *( _inputImage->getQImage() ), 6 );
        createSimilarityGraph();
    }
}


MainWindow::~MainWindow()
{
    delete _ui;
}


void MainWindow::createActions()
{
    _openAct = new QAction( tr( "&Open Pixel Art" ), this );
    _openAct->setShortcuts( QKeySequence::Open );
    _openAct->setStatusTip( tr( "Open Pixel Art Image" ) );
    connect( _openAct, SIGNAL( triggered() ), this, SLOT( loadImage() ) );

    _saveAct = new QAction( tr( "&Export Image" ), this );
    _saveAct->setShortcuts( QKeySequence::Save );
    _saveAct->setStatusTip( tr( "Export Depixelized Image" ) );
    connect( _saveAct, SIGNAL( triggered() ), this, SLOT( saveImage() ) );

    _exitAct = new QAction( tr( "&Exit" ), this );
    _exitAct->setShortcuts( QKeySequence::Save );
    _exitAct->setStatusTip( tr( "Exit" ) );
    connect( _exitAct, SIGNAL( triggered() ), this, SLOT( close() ) );

    _aboutAct = new QAction( tr( "&About" ), this );
    _aboutAct->setShortcuts( QKeySequence::Save );
    _aboutAct->setStatusTip( tr( "About" ) );
    connect( _aboutAct, SIGNAL( triggered() ), this, SLOT( aboutDialog() ) );
}


void MainWindow::createMenus()
{
    _fileMenu = _ui->menuBar->addMenu( tr( "&File" ) );
    _fileMenu->addAction( _openAct );
    _fileMenu->addAction( _saveAct );

    _fileMenu->addSeparator();

    _fileMenu->addAction( _exitAct );

    _editMenu = _ui->menuBar->addMenu( tr( "&Edit" ) );

    _helpMenu = _ui->menuBar->addMenu( tr( "&Help" ) );
    _helpMenu->addAction( _aboutAct );
}


void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                                     tr( "Open Image" ), "/home/",
                                                     tr( "Image Files (*.png *.jpg *.bmp)" ) );

    if( fileName == 0 )
    {
        return;
    }

    QImage* qimage = new QImage( fileName );

    if( qimage->size().width() * qimage->size().height() > 160000 )
    {
        QMessageBox* dialog = new QMessageBox();
        dialog->setWindowTitle( "Warning!" );
        dialog->setText( "Image size too big! (Remember, it must be pixel art)" );
        dialog->show();
        return;
    }

    _inputImage = new Image( qimage );

    fillLabels( *_inputImage );

    fillQGraphicsView( *qimage, 1 );

    fillQGraphicsViewOriginal( *qimage, 6 );

    createSimilarityGraph();

    enableFiltersFrame();

    _ui->radioButtonOriginal->setChecked( true );
}


void MainWindow::enableFiltersFrame()
{
    if( _inputImage == 0 )
    {
        _ui->frameFiltersOptions->setDisabled( true );
    }
    else
    {
        _ui->frameFiltersOptions->setEnabled( true );
    }
}


void MainWindow::fillQGraphicsView( QImage& qimage, u_int scaleFactor )
{
    if( &qimage == 0 )
    {
        return;
    }

    if( _resultScene != nullptr )
    {
        delete _resultScene;
    }
    _resultScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( qimage.scaled( scaleFactor * qimage.size() ) );
    _resultScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _resultScene );
}


void MainWindow::fillQGraphicsViewOriginal( QImage& qimage, u_int scaleFactor )
{
    if( &qimage == 0 )
    {
        return;
    }

    if( _originalScene != nullptr )
    {
        delete _originalScene;
    }
    _originalScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( qimage.scaled( scaleFactor * qimage.size() ) );
    _originalScene->addPixmap( pixmap );
    _ui->graphicsViewOriginal->setScene( _originalScene );
}


void MainWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName( this,
                                                     tr( "Save Image" ), "/home/",
                                                     tr( "Image Files (*.png *.jpg *.bmp)" ) );

    if( fileName == 0 )
    {
        return;
    }

    _outputImage->getQImage()->save( fileName );
}


void MainWindow::loadOriginal()
{
    fillQGraphicsView( *( _inputImage->getQImage() ), 1 );
}


void MainWindow::createTest()
{
    _inputImage = new Image( 256, 256 );
    _inputImage->createTest();

    //fillQGraphicsView( *( _inputImage->getQImage() ) );

    QGraphicsScene* scene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( _inputImage->getQImage()->scaled( 1 * _inputImage->getQImage()->size() ) );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}


void MainWindow::aboutDialog()
{
    QMessageBox* dialog = new QMessageBox();
    dialog->setWindowTitle( "About" );
    dialog->setText( "  Depixelator - @marcoc2" );
    dialog->show();
}


void MainWindow::fillLabels( Image image )
{
    //_ui->LabelName = image.getName;
    _ui->labelSize->setText( QString::number( image.getSize() ) );
    _ui->labelWidth->setText( QString::number( image.getWidth() ) );
    _ui->labelHeight->setText( QString::number( image.getHeight() ) );
}


void MainWindow::applyNearest()
{
    if( _inputImage == 0 || !_ui->radioButtonNearest->isChecked() )
    {
        return;
    }

    QImage scaledImage( _inputImage->getQImage()->scaled( _ui->spinBoxNearestScaleFactor->value() * _inputImage->getQImage()->size() ) );

    if( _resultScene != nullptr )
    {
        delete _resultScene;
    }
    _resultScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( scaledImage );
    _resultScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _resultScene );
}


void MainWindow::applyHqx()
{
    if( _inputImage == 0 )
    {
        return;
    }

    HqxFilter hqxFilter( _inputImage, 4.0f );

    applyAndShowOutputImage( hqxFilter );
}


void MainWindow::applyXbr()
{
    if( _inputImage == 0 )
    {
        return;
    }

    XbrFilter xbrFilter( _inputImage, 4.0f );

    applyAndShowOutputImage( xbrFilter );
}


void MainWindow::applyXbrZ()
{
    if( _inputImage == 0 )
    {
        return;
    }

    XbrZFilter xbrZFilter( _inputImage, 4.0f );

    applyAndShowOutputImage( xbrZFilter );
}


void MainWindow::applyCRT()
{
    if( _inputImage == 0 || !_ui->radioButtonCRT->isChecked() )
    {
        return;
    }

    CRTFilter crtFilter( _inputImage, ( float ) _ui->spinBoxScaleFactor->value() );

    applyAndShowOutputImage( crtFilter );
}


void MainWindow::applyScale2x()
{
    if( _inputImage == 0 )
    {
        return;
    }

    _outputImage = new Image( _inputImage->getWidth() * 2, _inputImage->getHeight() * 2 );

    Image* output4xImage = new Image( _inputImage->getWidth() * 4, _inputImage->getHeight() * 4 );

    Scale2xFilter* scale2xFilter = new Scale2xFilter( _inputImage, _outputImage );
    scale2xFilter->apply2x( output4xImage );

    if( _resultScene != nullptr )
    {
        _resultScene = new QGraphicsScene( this );
    }
    _resultScene = new QGraphicsScene( this );
    QImage* qimage = output4xImage->getQImage();
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    _resultScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _resultScene );
}


void MainWindow::applyEagle()
{
    if( _inputImage == 0 )
    {
        return;
    }

    EagleFilter eagleFilter( _inputImage, 2.0f );

    applyAndShowOutputImage( eagleFilter );
}


void MainWindow::applyAndShowOutputImage( Filter& filter )
{
    filter.apply();

    if( _resultScene != nullptr )
    {
        _resultScene = new QGraphicsScene( this );
    }

    if( _outputImage != nullptr )
    {
        //delete _outputImage;
    }
    _outputImage = new Image( filter.getOutputImage()->getQImage() );

    QImage* qimage = filter.getOutputImage()->getQImage();
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    _resultScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _resultScene );
}


void MainWindow::createSimilarityGraph()
{
    if( _inputImage == 0 )
    {
        return;
    }

    _similarityGraph = new SimilarityGraph( _inputImage );
    _similarityGraph->createGraph();

    _graphScene = new QGraphicsScene( this );
    QPen pen( QBrush( Qt::SolidPattern ), 1.5 );

    for( unsigned int i = 0; i < _inputImage->getHeight(); i++ )
    {
        for( unsigned int j = 0; j < _inputImage->getWidth(); j++ )
        {
            std::vector< SimilarityGraph::Point2D > lines = _similarityGraph->getNodeLines( j, i );
            for( unsigned int t = 0; t < lines.size(); t = t + 2 )
            {
                _graphScene->addLine( QLine( QPoint( lines[ t ].x, lines[ t ].y ), QPoint( lines[ t + 1 ].x,
                                                                                     lines[ t + 1 ].y ) ), pen );
            }
        }
    }


    _ui->graphicsViewGraph->setScene( _graphScene );
}


void MainWindow::applyVector()
{
    if( _inputImage == 0 )
    {
        return;
    }

    // Clear graphics view
    QGraphicsScene* scene = new QGraphicsScene( this );
    _ui->graphicsView->setScene( scene );

    QList< RenderArea* > renderAreas;

    Diagram* _diagram = new Diagram();
    _diagram->createDiagram( _similarityGraph );

    QPen pen( QBrush( Qt::SolidPattern ), 1.5 );

    std::vector< vector< Point2D > > polygons = _diagram->getPolygons();

    for( unsigned int i = 0; i < _inputImage->getHeight(); i++ )
    {
        for( unsigned int j = 0; j < _inputImage->getWidth(); j++ )
        {
            float scale = 4.0f;
            float xOffset = ( ( j ) * scale );
            float yOffset = ( ( i ) * scale );

            QPainterPath polygonPath;
            polygonPath.moveTo( j * scale, i * scale );

            int index = ( i * _inputImage->getWidth() + j );
            std::vector< Point2D > polygon = polygons[ index ];

            for( unsigned int p = 0; p < polygon.size(); p++ )
            {
                //polygonPath.lineTo( xOffset + scale * polygon[ p ].x, yOffset + scale * polygon[ p ].y );
                polygonPath.lineTo( xOffset + scale * polygon[ p ].x, yOffset + scale * polygon[ p ].y );
            }
            polygonPath.closeSubpath();

            RenderArea* rArea = new RenderArea( polygonPath );
            rArea->setPenColor( QColor( ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).red,
                                        ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).green,
                                        ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).blue ) );

            rArea->setFillGradient( QColor( ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).red,
                                            ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).green,
                                            ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).blue ),
                                    QColor( ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).red,
                                            ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).green,
                                            ( unsigned int ) ( uchar ) _inputImage->getPixel( j, i ).blue ) );

            rArea->setRotationAngle( 180 );

            rArea->setPenWidth( 0 );

            renderAreas.push_back( rArea );
        }
    }

    int i = 0;
    for( QList< RenderArea* >::iterator it = renderAreas.begin(); it != renderAreas.end(); it++, i++ )
    {
        _ui->gridLayout->addWidget( *it, 0, 0 );
    }
}


void MainWindow::resizeEvent( QResizeEvent* event )
{
    if( event->oldSize().width() == -1 )
    {
        return;
    }

    _ui->graphicsViewOriginal->resize( width() / 3.5,
                                       height() / 2.35 );

    _ui->graphicsViewGraph->resize( width() / 3.5,
                                    height() / 2.35 );

    _ui->graphicsViewGraph->move( _ui->graphicsViewGraph->pos().x(),
                                  _ui->graphicsViewOriginal->pos().y() + _ui->graphicsViewOriginal->height() + 25 );

    _ui->labelSimilarityGraphView->move( _ui->labelSimilarityGraphView->pos().x(),
                                         _ui->graphicsViewOriginal->pos().y() + _ui->graphicsViewOriginal->height() + 5 );

    _ui->graphicsView->move( _ui->graphicsViewOriginal->pos().x() + _ui->graphicsViewOriginal->width() + 5,
                             _ui->graphicsView->pos().y() );

    _ui->labelFilteredView->move( _ui->graphicsViewOriginal->pos().x() + _ui->graphicsViewOriginal->width() + 5,
                                  _ui->labelFilteredView->pos().y() );

    _ui->graphicsView->resize( width() / 1.75,
                               height() - 90 );

    QWidget::resizeEvent(event);
}
