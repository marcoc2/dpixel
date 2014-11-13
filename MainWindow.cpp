#include <stdio.h>

#include <QFileDialog>
#include <QMessageBox>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _inputImage( 0 )
{
    _inputImage = new Image( "/home/marco/Dropbox/Programação/Mestrado/Estudo Orientado/graph_cv_gl/alex.png" );

    _ui->setupUi(this);

    connect(_ui->testButton, SIGNAL(clicked()), this, SLOT(createTest()));
    connect(_ui->loadImageButton, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(_ui->saveImageButton, SIGNAL(clicked()), this, SLOT(saveImage()));

    connect(_ui->radioButtonOriginal, SIGNAL(clicked()), this, SLOT(loadOriginal()));
    connect(_ui->radioButtonNearest, SIGNAL(clicked()), this, SLOT(applyNearest()));
    connect(_ui->radioButtonHqx, SIGNAL(clicked()), this, SLOT(applyHqx()));
    connect(_ui->radioButtonXbr, SIGNAL(clicked()), this, SLOT(applyXbr()));
    connect(_ui->radioButtonXbrZ, SIGNAL(clicked()), this, SLOT(applyXbrZ()));
    connect(_ui->radioButtonScale2x, SIGNAL(clicked()), this, SLOT(applyScale2x()));
    connect(_ui->radioButtonEagle, SIGNAL(clicked()), this, SLOT(applyEagle()));
    connect(_ui->radioButtonVector, SIGNAL(clicked()), this, SLOT(applyVector()));

    enableFiltersFrame();

    createActions();
    createMenus();

    if( _inputImage != 0)
    {
        fillQGraphicsView( *( _inputImage->getQImage() ) );
        createSimilarityGraph();
    }

}



MainWindow::~MainWindow()
{
    delete _ui;
}



void MainWindow::createActions()
{
    _openAct = new QAction(tr("&Open Pixel Art"), this);
    _openAct->setShortcuts(QKeySequence::Open);
    _openAct->setStatusTip(tr("Open Pixel Art Image"));
    connect( _openAct, SIGNAL(triggered()), this, SLOT( loadImage() ) );

    _saveAct = new QAction(tr("&Export Image"), this);
    _saveAct->setShortcuts(QKeySequence::Save);
    _saveAct->setStatusTip(tr("Export Depixelized Image"));
    connect( _saveAct, SIGNAL(triggered()), this, SLOT( saveImage() ) );

    _exitAct = new QAction(tr("&Exit"), this);
    _exitAct->setShortcuts(QKeySequence::Save);
    _exitAct->setStatusTip(tr("Exit"));
    connect( _exitAct, SIGNAL(triggered()), this, SLOT( close() ) );

    _aboutAct = new QAction(tr("&About"), this);
    _aboutAct->setShortcuts(QKeySequence::Save);
    _aboutAct->setStatusTip(tr("About"));
    connect( _aboutAct, SIGNAL(triggered()), this, SLOT( aboutDialog() ) );
}



void MainWindow::createMenus()
{
    _fileMenu = _ui->menuBar->addMenu(tr("&File"));
    _fileMenu->addAction( _openAct );
    _fileMenu->addAction( _saveAct );

    _fileMenu->addSeparator();

    _fileMenu->addAction( _exitAct );

    _editMenu = _ui->menuBar->addMenu(tr("&Edit"));

    _helpMenu = _ui->menuBar->addMenu(tr("&Help"));
    _helpMenu->addAction( _aboutAct );
}



void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName == 0)
        return;

    QImage* qimage = new QImage( fileName );

    if(qimage->size().width() * qimage->size().height() > 160000 )
    {
        QMessageBox* dialog = new QMessageBox();
        dialog->setWindowTitle( "Warning!" );
        dialog->setText( "Image size too big! (Remember, it must be pixel art)" );
        dialog->show();
        return;
    }

    _inputImage = new Image( qimage );

    fillLabels( *_inputImage );

    fillQGraphicsView( *qimage );

    createSimilarityGraph();

    enableFiltersFrame();
}



void MainWindow::enableFiltersFrame()
{
    if( _inputImage == 0 )
        _ui->frameFiltersOptions->setDisabled( true );
    else
        _ui->frameFiltersOptions->setEnabled( true );

}



void MainWindow::fillQGraphicsView( QImage& qimage, u_int scaleFactor )
{
    if( &qimage == 0 )
        return;

    QGraphicsScene *scene = new QGraphicsScene (this);
    QPixmap pixmap = QPixmap::fromImage( qimage.scaled( scaleFactor * qimage.size() ) );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
    _ui->graphicsViewOriginal->setScene( scene );
}



void MainWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName( this,
        tr("Save Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName == 0)
        return;

    _outputImage->getQImage()->save( fileName );
}



void MainWindow::loadOriginal()
{
    fillQGraphicsView( *(_inputImage->getQImage()) );
}



void MainWindow::createTest()
{
    _inputImage = new Image( 256, 256 );
    _inputImage->createTest();

    //fillQGraphicsView( *( _inputImage->getQImage() ) );

    QGraphicsScene *scene = new QGraphicsScene (this);
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
    _ui->labelWidth->setText( QString::number(  image.getWidth() ) );
    _ui->labelHeight->setText( QString::number( image.getHeight() ) );
}



void MainWindow::applyNearest()
{
    if( _inputImage == 0 )
        return;

    QImage* scaledImage = new QImage( _inputImage->getQImage()->scaled( 16 * _inputImage->getQImage()->size() ) );

    QGraphicsScene *scene = new QGraphicsScene (this);
    QPixmap pixmap = QPixmap::fromImage( *scaledImage );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}



void MainWindow::applyHqx()
{
    if( _inputImage == 0 )
        return;

    _outputImage = new Image( _inputImage->getWidth() * 4, _inputImage->getHeight() * 4);
    HqxFilter* hqxFilter = new HqxFilter( _inputImage, _outputImage );
    hqxFilter->apply();

    QGraphicsScene *scene = new QGraphicsScene (this);
    QImage* qimage = _outputImage->getQImage();
    //qimage->save( "/home/marco/Desktop/asdf.png" );
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}



void MainWindow::applyXbr()
{
    if( _inputImage == 0 )
        return;

    _outputImage = new Image( _inputImage->getWidth() * 4, _inputImage->getHeight() * 4);
    XbrFilter* xbrFilter = new XbrFilter( _inputImage, _outputImage );
    xbrFilter->apply();

    QGraphicsScene *scene = new QGraphicsScene (this);
    QImage* qimage = _outputImage->getQImage();
    //qimage->save( "/home/marco/Desktop/asdf.png" );
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}



void MainWindow::applyXbrZ()
{
    if( _inputImage == 0 )
        return;

    _outputImage = new Image( _inputImage->getWidth() * 4, _inputImage->getHeight() * 4);
    XbrZFilter* xbrZFilter = new XbrZFilter( _inputImage, _outputImage );
    xbrZFilter->apply();

    QGraphicsScene *scene = new QGraphicsScene (this);
    QImage* qimage = _outputImage->getQImage();
    qimage->save( "/home/marco/Desktop/ness_xbrz.png" );
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}



void MainWindow::applyScale2x()
{
    if( _inputImage == 0 )
        return;

    _outputImage = new Image( _inputImage->getWidth() * 2, _inputImage->getHeight() * 2 );

    Image* output4xImage = new Image( _inputImage->getWidth() * 4, _inputImage->getHeight() * 4 );

    Scale2xFilter* scale2xFilter = new Scale2xFilter( _inputImage, _outputImage );
    scale2xFilter->apply2x( output4xImage );

    QGraphicsScene *scene = new QGraphicsScene (this);
    QImage* qimage = output4xImage->getQImage();
    //qimage->save( "/home/marco/Desktop/asdf.png" );
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}


void MainWindow::applyEagle()
{
    if( _inputImage == 0 )
        return;

    _outputImage = new Image( _inputImage->getWidth() * 2, _inputImage->getHeight() * 2 );

    //Image* output4xImage = new Image( _inputImage->getWidth() * 4, _inputImage->getHeight() * 4 );

    EagleFilter* eagleFilter = new EagleFilter( _inputImage, _outputImage );

    eagleFilter->apply();
    //scale2xFilter->apply2x( output4xImage );

    QGraphicsScene *scene = new QGraphicsScene (this);
    QImage* qimage = _outputImage->getQImage();
    //qimage->save( "/home/marco/Desktop/asdf.png" );
    QPixmap pixmap = QPixmap::fromImage( *qimage );
    scene->addPixmap( pixmap );
    _ui->graphicsView->setScene( scene );
}



void MainWindow::createSimilarityGraph()
{
    if( _inputImage == 0 )
        return;

    _similarityGraph = new SimilarityGraph( _inputImage );
    _similarityGraph->createGraph();

    QGraphicsScene *scene = new QGraphicsScene (this);
    QPen pen( QBrush( Qt::SolidPattern ), 1.5 );

    for(unsigned int i = 0 ; i < _inputImage->getHeight() ; i++ )
    {
        for(unsigned int j = 0 ; j < _inputImage->getWidth() ; j++ )
        {
            std::vector<SimilarityGraph::Point2D> lines = _similarityGraph->getNodeLines( j, i );
            for(unsigned int t = 0; t < lines.size(); t = t+2)
                scene->addLine( QLine(QPoint( lines[t].x, lines[t].y), QPoint( lines[t+1].x, lines[t+1].y )), pen );
        }
    }


    _ui->graphicsViewGraph->setScene( scene );
}



void MainWindow::applyVector()
{
    if( _inputImage == 0 )
        return;

    // Clear graphics view
    QGraphicsScene *scene = new QGraphicsScene (this);
    _ui->graphicsView->setScene(scene);

    QList<RenderArea*> renderAreas;

    Diagram* _diagram = new Diagram();
    _diagram->createDiagram( _similarityGraph );

    QPen pen( QBrush( Qt::SolidPattern ), 1.5 );

    std::vector< vector< Point2D > > polygons = _diagram->getPolygons();

    for(unsigned int i = 0 ; i < _inputImage->getHeight() ; i++ )
    {
        for(unsigned int j = 0 ; j < _inputImage->getWidth() ; j++ )
        {
            float scale = 4.0f;
            float xOffset = ((j) * scale);
            float yOffset = ((i) * scale);

            QPainterPath polygonPath;
            polygonPath.moveTo(j * scale, i * scale);

            int index = (i*_inputImage->getWidth() + j);
            std::vector< Point2D > polygon = polygons[index];

            for( unsigned int p = 0; p < polygon.size(); p++ )
            {
                //polygonPath.lineTo( xOffset + scale * polygon[ p ].x, yOffset + scale * polygon[ p ].y );
                polygonPath.lineTo( xOffset + scale * polygon[ p ].x, yOffset + scale * polygon[ p ].y );
            }
            polygonPath.closeSubpath();

            RenderArea *rArea = new RenderArea(polygonPath);
            rArea->setPenColor(QColor((unsigned int) (uchar) _inputImage->getPixel( j, i).red,
                                     (unsigned int) (uchar) _inputImage->getPixel( j, i).green,
                                     (unsigned int) (uchar) _inputImage->getPixel( j, i).blue ) );

            rArea->setFillGradient(QColor((unsigned int) (uchar) _inputImage->getPixel( j, i).red,
                                          (unsigned int) (uchar) _inputImage->getPixel( j, i).green,
                                          (unsigned int) (uchar) _inputImage->getPixel( j, i).blue ),
                                   QColor((unsigned int) (uchar) _inputImage->getPixel( j, i).red,
                                         (unsigned int) (uchar) _inputImage->getPixel( j, i).green,
                                         (unsigned int) (uchar) _inputImage->getPixel( j, i).blue ) );

            rArea->setRotationAngle(180);

            rArea->setPenWidth(0);

            renderAreas.push_back( rArea );
        }
    }

    int i=0;
    for(QList<RenderArea*>::iterator it = renderAreas.begin(); it != renderAreas.end(); it++, i++)
        _ui->gridLayout->addWidget(*it, 0, 0);

}
