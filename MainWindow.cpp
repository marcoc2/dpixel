/*
 * 2016 - Marco Aurélio G. da Silva ( marcoc2@gmail.com )
 */

#include <stdio.h>
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QImageReader>
#include <QDesktopWidget>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CheckUpscaleWindow.h"

#include "Widgets/RenderArea.h"

#include "Depixel/SimilarityGraph.h"
#include "Depixel/Diagram.h"
#include "Filters/hqx/HqxFilter.h"
#include "Filters/xBR/XbrFilter.h"
#include "Filters/xBRZ/XbrZFilter.h"
#include "Filters/Scale2xFilter.h"
#include "Filters/EagleFilter.h"
#include "Filters/CRTFilter.h"
#include "Filters/Super2xSal.h"
#include "ImageOperations/CheckUpscale.h"
#include "GifSaver.h"

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    _ui( new Ui::MainWindow ),
    _inputImage( nullptr ),
    _outputImage( nullptr ),
    _similarityGraph( nullptr ),
    _currentFilter( nullptr ),
    _resultScene( nullptr ),
    _originalScene( nullptr ),
    _graphScene( nullptr ),
    _gifSaver( nullptr ),
    _isAnimatedGif( false )
{
    #ifdef _WIN32
    _inputImage = new Image( "../Samples/metalslug.png" );
    #else
    _inputImage = new Image( "../Samples/metalslug.png" );
    _currentFileName = "../Samples/metalslug.png";
    #endif

    _ui->setupUi( this );

    connectSignals();
    createActions();
    createMenus();
    _ui->filterProgressBar->setVisible( false );
    _ui->filteredGLWidget->setVisible( false );

    if( !_inputImage->getQImage()->isNull() )
    {
        initialize();
    }
    else
    {
        _inputImage = 0;
    }

    // Center window position
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = ( screenGeometry.width() - this->width() ) / 2;
    int y = ( screenGeometry.height() - this->height() ) / 2;
    this->move( x, y );
    this->show();
}


void MainWindow::connectSignals()
{
    connect( _ui->testButton, SIGNAL( clicked() ), this, SLOT( createTest() ) );
    connect( _ui->glButton, SIGNAL( clicked() ), this, SLOT( enableOpenGLFrontEnd() ) );
    connect( _ui->loadImageButton, SIGNAL( clicked() ), this, SLOT( loadImage() ) );
    connect( _ui->saveImageButton, SIGNAL( clicked() ), this, SLOT( saveImage() ) );
    connect( _ui->exportGIFButton, SIGNAL( clicked() ), this, SLOT( saveAnimatedGif() ) );

    _ui->exportGIFButton->setVisible( false );

    connect( _ui->radioButtonOriginal, SIGNAL( clicked() ), this, SLOT( loadOriginal() ) );
    connect( _ui->radioButtonNearest, SIGNAL( clicked() ), this, SLOT( applyNearest() ) );
    connect( _ui->radioButtonBilinear, SIGNAL( clicked() ), this, SLOT( applyBilinear() ) );
    connect( _ui->radioButtonHqx, SIGNAL( clicked() ), this, SLOT( applyHqx() ) );
    connect( _ui->radioButtonXbr, SIGNAL( clicked() ), this, SLOT( applyXbr() ) );
    connect( _ui->radioButtonXbrZ, SIGNAL( clicked() ), this, SLOT( applyXbrZ() ) );
    connect( _ui->radioButtonScale2x, SIGNAL( clicked() ), this, SLOT( applyScale2x() ) );
    connect( _ui->radioButtonEagle, SIGNAL( clicked() ), this, SLOT( applyEagle() ) );
    connect( _ui->radioButtonCRT, SIGNAL( clicked() ), this, SLOT( applyCRT() ) );
    connect( _ui->radioButtonVector, SIGNAL( clicked() ), this, SLOT( applyVector() ) );
    connect( _ui->radioButtonSuperSaI2x, SIGNAL( clicked() ), this, SLOT( applySuperSaI2x() ) );

    connect( _ui->spinBoxNearestScaleFactor, SIGNAL( valueChanged( int ) ), this, SLOT( applyNearest() ) );
    connect( _ui->spinBoxBilinearScaleFactor, SIGNAL( valueChanged( int ) ), this, SLOT( applyBilinear() ) );
    connect( _ui->spinBoxScaleFactor, SIGNAL( valueChanged( int ) ), this, SLOT( applyCRT() ) );
    connect( _ui->scale2xSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( applyScale2x() ) );
    connect( _ui->eagleSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( applyEagle() ) );
    connect( _ui->xBRZSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( applyXbrZ() ) );
    connect( _ui->xBRSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( applyXbr() ) );

    // Hide filters with issues and resize frame
    _ui->glButton->setVisible( false );
    _ui->testButton->setVisible( false );
    _ui->radioButtonVector->setVisible( false );
    _ui->radioButtonSuperSaI2x->setVisible( false );
    _ui->frameFiltersOptions->resize(
                _ui->frameFiltersOptions->size().width(),
                _ui->frameFiltersOptions->size().height() - 50 );
}


MainWindow::~MainWindow()
{
    delete _ui;
    delete _similarityGraph;
    delete _inputImage;
    delete _outputImage;
    delete _currentFilter;
    clearGifHolder();
}


void MainWindow::initialize()
{
    createSimilarityGraph();

    int resizedFactor = 1; //CheckUpscale::checkUpscale( _similarityGraph );
    if( resizedFactor < 1 )
    {
        reloadResizedImage( 1.0 / ( float ) resizedFactor );
        createSimilarityGraph();

        QMessageBox* dialog = new QMessageBox();
        dialog->setWindowTitle( "Warning!" );
        std::stringstream s;
        s << "The loaded image seems to be upscaled " << resizedFactor << " times.\nImage auto downscaled";
        dialog->setText( s.str().c_str() );
        dialog->show();
    }
    else
    {
        const int limit = 100;
        if( _inputImage->getSize() < limit * limit )
        {
            new CheckUpscaleWindow( this, new Image( new QImage( *( _inputImage->getQImage() ) ) ) );
        }
        else
        {
            int midX = _inputImage->getWidth() / 2;
            int midY = _inputImage->getHeight() / 2;
            Image* cropedImage = new Image( limit, limit );
            for( size_t j = 0; j < limit; j++ )
            {
                for( size_t i = 0; i < limit; i++ )
                {
                    int i_ = i + midX - limit / 2;
                    int j_ = j + midY - limit / 2;
                    const Pixel& pixel = _inputImage->getPixel( i_, j_ );
                    cropedImage->setPixel( i, j, pixel );
                }
            }
            cropedImage->fillQImageRGB();
            new CheckUpscaleWindow( this, cropedImage );
        }
    }

    fillLabels( _inputImage );
    fillQGraphicsView( *( _inputImage->getQImage() ), 1 );
    fillQGraphicsViewOriginal( *( _inputImage->getQImage() ) );
    enableFiltersFrame();

    _ui->radioButtonOriginal->setChecked( true );

    if( _isAnimatedGif )
    {
        _ui->exportGIFButton->setVisible( true );
    }
    else
    {
        _ui->exportGIFButton->setVisible( false );
    }
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

    _exportSimilarityGraphAct = new QAction( tr( "Export Similarity &Graph" ), this );
    _exportSimilarityGraphAct->setStatusTip( tr( "Export Similatity Graph" ) );
    connect( _exportSimilarityGraphAct, SIGNAL( triggered() ), this, SLOT( exportSimilarityGraph() ) );

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
    _fileMenu->addAction( _exportSimilarityGraphAct );

    _fileMenu->addSeparator();

    _fileMenu->addAction( _exitAct );

    _editMenu = _ui->menuBar->addMenu( tr( "&Edit" ) );

    _helpMenu = _ui->menuBar->addMenu( tr( "&Help" ) );
    _helpMenu->addAction( _aboutAct );
}


void MainWindow::enableOpenGLFrontEnd()
{
    if( !checkCurrentFilter( true ) )
    {
        return;
    }

    if( !(_ui->filteredGLWidget->isVisible()) )
    {
        //_ui->filteredGLWidget->setTexture( *( _outputImage->getQImage() ) );
        _ui->filteredGLWidget->setVisible( true );
        _ui->filteredGLWidget->move( _ui->graphicsView->pos().x(), _ui->graphicsView->pos().y() );
        _ui->filteredGLWidget->resize( _ui->graphicsView->width(), _ui->graphicsView->height() );
    }
    else
    {
        _ui->filteredGLWidget->setVisible( false );
    }

    if( _ui->filteredGLWidget->getOpenGLVersion() < 4 )
    {
        //QMessageBox* dialog = new QMessageBox();
        //dialog->setWindowTitle( "Warning!" );
        //dialog->setText( "OpenGL 4.0 not supported." );
        //dialog->show();
        //_ui->filteredGLWidget->setVisible( false );
    }
}


void MainWindow::loadImage()
{
    _currentFileName = QFileDialog::getOpenFileName( this,
                                                     tr( "Open Image" ), "/home/",
                                                     tr( "Image Files (*.png *.jpg *.bmp *.gif)" ) );

    if( _currentFileName == 0 )
    {
        return;
    }

    QImage* qimage = new QImage( _currentFileName );

    if( qimage->size().width() * qimage->size().height() > 1500 * 1500 )
    {
        QMessageBox* dialog = new QMessageBox();
        dialog->setWindowTitle( "Warning!" );
        dialog->setText( "Image size too big! (Remember, it must be pixel art)" );
        dialog->show();

        delete qimage;
        return;
    }

    QImageReader qImageReader( _currentFileName );
    if( qImageReader.supportsAnimation() && ( qImageReader.imageCount() > 1 ) )
    {
        _isAnimatedGif = true;
        if( _inputAnimatedGif.size() > 0 )
        {
            clearGifHolder();
        }

        loadAnimatedGifHolder( qImageReader );

        std::stringstream s;
        s << "Animated gif containing " << qImageReader.imageCount() << " frames" << std::endl;
        s << "You can only preview the first frame, but can export the filtered animated gif";
        QMessageBox dialog;
        dialog.setWindowTitle( "Warning!" );
        dialog.setText( s.str().c_str() );
        dialog.show();
    }
    else
    {
        _isAnimatedGif = false;
    }

    _inputImage = new Image( qimage );

    initialize();
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


void MainWindow::fillQGraphicsViewOriginal( QImage& qimage )
{
    if( &qimage == 0 )
    {
        return;
    }

    if( _originalScene != nullptr )
    {
        delete _originalScene;
    }
    int scaleFactor = 4;
    if( qimage.width() * qimage.height() > 128 * 128 )
    {
        scaleFactor = 2;
    }
    _originalScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( qimage.scaled( scaleFactor * qimage.size() ) );
    _originalScene->addPixmap( pixmap );
    _ui->graphicsViewOriginal->setScene( _originalScene );
}


QString MainWindow::getSuggestedFileName( QString format )
{
    return QFileInfo( _currentFileName ).absoluteDir().absolutePath() +
                QString( "/" ) + QFileInfo( _currentFileName ).baseName() + QString( "_" ) +
                QString( _currentFilter->getName().c_str() ) + QString( "_" ) +
                QString::number( _currentFilter->getScaleFactor() ) + QString( "x." ) +
                format;
}


void MainWindow::saveImage()
{
    if( !checkCurrentFilter( true ) )
    {
        return;
    }

    QString outputFileName =
            QFileDialog::getSaveFileName( this,
                                          tr( "Save Image" ),
                                          getSuggestedFileName( "png" ).toStdString().c_str(),
                                          tr( "Image Files (*.png *.jpg *.bmp)" ) );

    if( outputFileName == 0 )
    {
        return;
    }

    _outputImage->getQImage()->save( outputFileName );
}


void MainWindow::saveAnimatedGif()
{
    if( !checkCurrentFilter( true ) )
    {
        return;
    }

    QString outputFileName =
            QFileDialog::getSaveFileName( this,
                                          tr( "Save Image" ),
                                          getSuggestedFileName( "gif" ).toStdString().c_str(),
                                          tr( "Image Files (*.gif)" ) );

    if( outputFileName == 0 )
    {
        return;
    }

    _gifSaver = new GifSaver( outputFileName, _currentFilter,
                                       _currentFilter->getScaleFactor(),
                                       _inputAnimatedGif );
    connect( _gifSaver, SIGNAL( finished()) , this, SLOT( finishSaveGif() ) );
    connect( _gifSaver, SIGNAL( setProgress( int ) ), this, SLOT( setProgress( int ) ) );
    _gifSaver->start( QThread::NormalPriority );

    _ui->filterProgressBar->setVisible( true );
    setEnabled( false );
}


void MainWindow::exportSimilarityGraph()
{
    QString suggestedFileName = QFileInfo( _currentFileName ).absoluteDir().absolutePath() +
            QString( "/" ) + QFileInfo( _currentFileName ).baseName() + QString( "_" ) +
            QString( "similarity_graph.png" );


    QString outputFileName =
            QFileDialog::getSaveFileName( this,
                                          tr( "Save Image" ), suggestedFileName,
                                          tr( "Image Files (*.png)" ) );

    if( outputFileName == 0 )
    {
        return;
    }

    QSize originalSize = _ui->graphicsViewGraph->size();
    QSize graphSize = _inputImage->getQImage()->size() * 10;
    _ui->graphicsViewGraph->resize( graphSize );
    QPixmap pixmap = _ui->graphicsViewGraph->grab( QRect( QPoint( 0,0 ),
                                                          QSize( graphSize.width() - 15,
                                                                 graphSize.height() - 15 ) ) );
    _ui->graphicsViewGraph->resize( originalSize );
    pixmap.save( outputFileName );
}


bool MainWindow::checkCurrentFilter( bool showWarning )
{
    if( _currentFilter == nullptr )
    {
        if( showWarning )
        {
            QMessageBox* dialog = new QMessageBox();
            dialog->setWindowTitle( "Warning!" );
            dialog->setText( "Select a filter first" );
            dialog->show();
        }
        return false;
    }
    else
    {
        if( !_currentFilter->isFinished() )
        {
            if( showWarning )
            {
                QMessageBox* dialog = new QMessageBox();
                dialog->setWindowTitle( "Warning!" );
                dialog->setText( "Still processing previous filter" );
                dialog->show();
            }
            return false;
        }
        return true;
    }
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
    dialog->setText( "dpixel - A Pixel Art Remaster Tool\n\nhttps://github.com/marcoc2/dpixel\n\nmarcoc2@gmail.com" );
    dialog->show();
}


void MainWindow::fillLabels( Image* image )
{
    QFileInfo fileInfo( _currentFileName );
    _ui->LabelName->setText( fileInfo.fileName() );
    _ui->labelSize->setText( QString::number( image->getSize() ) );
    _ui->labelWidth->setText( QString::number( image->getWidth() ) );
    _ui->labelHeight->setText( QString::number( image->getHeight() ) );

    _ui->statusBar->showMessage( _currentFileName );
}


void MainWindow::applyNearest()
{
    if( _inputImage == 0 || !_ui->radioButtonNearest->isChecked() )
    {
        return;
    }

    QImage scaledImage( _inputImage->getQImage()->scaled(
                            _ui->spinBoxNearestScaleFactor->value() * _inputImage->getQImage()->size() ) );

    if( _resultScene != nullptr )
    {
        delete _resultScene;
    }
    _resultScene = new QGraphicsScene( this );
    QPixmap pixmap = QPixmap::fromImage( scaledImage );
    _resultScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _resultScene );
}


void MainWindow::applyBilinear()
{
    if( _inputImage == 0 || !_ui->radioButtonBilinear->isChecked() )
    {
        return;
    }

    QImage scaledImage( _inputImage->getQImage()->scaled(
                            _ui->spinBoxBilinearScaleFactor->value() * _inputImage->getQImage()->size(),
                            Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation ) );

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

    HqxFilter* hqxFilter = new HqxFilter( _inputImage, 4.0f );

    applyAndShowOutputImage( hqxFilter );
}


void MainWindow::applyXbrZ()
{
    if( _inputImage == 0 || !_ui->radioButtonXbrZ->isChecked() )
    {
        return;
    }

    XbrZFilter* xbrZFilter = new XbrZFilter( _inputImage, _ui->xBRZSpinBox->value() );

    applyAndShowOutputImage( xbrZFilter );
}


void MainWindow::applyXbr()
{
    if( _inputImage == 0 || !_ui->radioButtonXbr->isChecked() )
    {
        return;
    }

    XbrFilter* xbrFilter = new XbrFilter( _inputImage, _ui->xBRSpinBox->value() );

    applyAndShowOutputImage( xbrFilter );
}


void MainWindow::applyCRT()
{
    if( _inputImage == 0 || !_ui->radioButtonCRT->isChecked() )
    {
        return;
    }

    CRTFilter* crtFilter = new CRTFilter( _inputImage, _ui->spinBoxScaleFactor->value() );

    applyAndShowOutputImage( crtFilter );
}


void MainWindow::applyScale2x()
{
    if( _inputImage == 0 || !_ui->radioButtonScale2x->isChecked() )
    {
        return;
    }

    Scale2xFilter* scale2xFilter = new Scale2xFilter( _inputImage, ( int ) _ui->scale2xSpinBox->value() );

    applyAndShowOutputImage( scale2xFilter );
}


void MainWindow::applyEagle()
{
    if( _inputImage == 0 || !_ui->radioButtonEagle->isChecked() )
    {
        return;
    }

    EagleFilter* eagleFilter = new EagleFilter( _inputImage, ( int ) _ui->eagleSpinBox->value() );

    applyAndShowOutputImage( eagleFilter );
}


void MainWindow::applyAndShowOutputImage( Filter* filter )
{
    _ui->filterProgressBar->setVisible( true );
    _ui->frameFiltersOptions->setEnabled( false );

    _currentFilter = filter;
    connect( _currentFilter, SIGNAL( finished()) , this, SLOT( finishFilter() ) );
    connect( _currentFilter, SIGNAL( setProgress( int ) ), this, SLOT( setProgress( int ) ) );
    _currentFilter->start( QThread::NormalPriority );
}


void MainWindow::createSimilarityGraph()
{
    if( _inputImage == 0 )
    {
        return;
    }

    if( _similarityGraph != nullptr )
    {
        delete _similarityGraph;
    }

    _similarityGraph = new SimilarityGraph( _inputImage );
    _similarityGraph->createGraph();


    // Limit graph size cause drawing a QScene is very costly
    if( _similarityGraph->getWidth() * _similarityGraph->getHeight() < 160000 )
    {
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
    else
    {
        if( _graphScene )
        {
            _graphScene->clear();
            _ui->graphicsViewGraph->items().clear();
        }
    }

}


void MainWindow::applySuperSaI2x()
{
    if( _inputImage == 0 )
    {
        return;
    }

    Super2xSal* super2xSal = new Super2xSal( _inputImage, 1 );

    applyAndShowOutputImage( super2xSal );
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
        //_ui->gridLayout->addWidget( *it, 0, 0 );
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
                                         _ui->graphicsViewOriginal->pos().y() + _ui->graphicsViewOriginal->height() +
                                         5 );

    _ui->graphicsView->move( _ui->graphicsViewOriginal->pos().x() + _ui->graphicsViewOriginal->width() + 5,
                             _ui->graphicsView->pos().y() );

    _ui->fileInfoFrame->move( _ui->graphicsViewOriginal->pos().x() + _ui->graphicsViewOriginal->width() + 5,
                              _ui->graphicsView->pos().y() );

    _ui->labelFilteredView->move( _ui->graphicsViewOriginal->pos().x() + _ui->graphicsViewOriginal->width() + 5,
                                  _ui->labelFilteredView->pos().y() );

    _ui->graphicsView->resize( width() / 1.75,
                               height() - 100 );

    _ui->filterProgressBar->move( _ui->graphicsViewOriginal->pos().x() + _ui->graphicsViewOriginal->width() + 5,
                                  _ui->graphicsView->pos().y() + _ui->graphicsView->height() + 2 );

    _ui->filterProgressBar->resize( _ui->graphicsView->width(), _ui->filterProgressBar->height() );

    _ui->filteredGLWidget->move( _ui->graphicsView->pos().x(), _ui->graphicsView->pos().y() );
    _ui->filteredGLWidget->resize( _ui->graphicsView->width(), _ui->graphicsView->height() );

    QWidget::resizeEvent( event );
}


void MainWindow::reloadResizedImage( float resizedFactor )
{
    if( ( int ) resizedFactor == 1 )
    {
        return;
    }

    _inputImage->resize( resizedFactor );

    createSimilarityGraph();

    if( _isAnimatedGif )
    {
        for( const auto& image : _inputAnimatedGif )
        {
            image->resize( resizedFactor );
        }
    }
}


void MainWindow::downscaleInputImage( int factor )
{
    reloadResizedImage( 1.0f / ( float ) factor );

    fillQGraphicsView( *( _inputImage->getQImage() ), 1 );
    fillQGraphicsViewOriginal( *( _inputImage->getQImage() ) );
}


void MainWindow::finishFilter()
{
    // Indicates busy status bar to wait for the qt scene to be filled
    _ui->filterProgressBar->setMaximum( 0 );
    _ui->filterProgressBar->setMinimum( 0 );
    _ui->filterProgressBar->setValue( 0 );

    if( _resultScene != nullptr )
    {
        _resultScene = new QGraphicsScene( this );
    }

    if( _outputImage != nullptr )
    {
        delete _outputImage;
    }

    _outputImage = new Image( new QImage( *( _currentFilter->getOutputImage()->getQImage() ) ) );

    QImage* qimage = _currentFilter->getOutputImage()->getQImage();
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    _resultScene->addPixmap( pixmap );
    _ui->graphicsView->setScene( _resultScene );

    _ui->filterProgressBar->setMaximum( 100 );
    _ui->filterProgressBar->setMinimum( 0 );
    _ui->filterProgressBar->setValue( 0 );
    _ui->filterProgressBar->setVisible( false );
    _ui->frameFiltersOptions->setEnabled( true );
}


void MainWindow::finishSaveGif()
{
    setEnabled( true );
    _ui->filterProgressBar->setVisible( false );

    delete _gifSaver;
}


void MainWindow::setProgress( int percentage )
{
    _ui->filterProgressBar->setValue( percentage );
}


void MainWindow::loadAnimatedGifHolder( QImageReader& qImageReader )
{
    for( int i = 0; i < qImageReader.imageCount(); i++ )
    {
        qImageReader.jumpToImage( i );
        QImage* qFrame = new QImage( qImageReader.read() );
        Image* image = new Image( qFrame );
        _inputAnimatedGif.push_back( image );
    }
}


void MainWindow::clearGifHolder()
{
    for( auto const& frame : _inputAnimatedGif )
    {
        delete frame;
    }
    _inputAnimatedGif.clear();
}
