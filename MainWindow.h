/*
 * 2016 Marco Aurélio G. da Silva ( marcoc2@gmail.com )
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Image.h"
#include "Filters/Filter.h"
#include <QMainWindow>
#include <QRadioButton>
#include <QGraphicsView>

class SimilarityGraph;
class Filter;
class GifSaver;

namespace Ui
{
class MainWindow;
}

class MainWindow :
    public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief MainWindow
     * @param parent
     */
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

    void downscaleInputImage( int factor );

private:

    Ui::MainWindow* _ui;
    Image* _inputImage;
    Image* _outputImage;
    std::vector< Image* > _inputAnimatedGif;
    QString _currentFileName;
    SimilarityGraph* _similarityGraph;
    Filter* _currentFilter;
    QAction* _openAct;
    QAction* _saveAct;
    QAction* _exportSimilarityGraphAct;
    QAction* _exitAct;
    QAction* _aboutAct;
    QMenu* _fileMenu;
    QMenu* _editMenu;
    QMenu* _helpMenu;

    QGraphicsScene* _resultScene;
    QGraphicsScene* _originalScene;
    QGraphicsScene* _graphScene;

    GifSaver* _gifSaver;

    bool _isAnimatedGif;

    void connectSignals();
    void createActions();
    void createMenus();

    void fillQGraphicsView( QImage& qimage, u_int scaleFactor = 8 );
    void fillQGraphicsViewOriginal(QImage& qimage );
    void applyAndShowOutputImage( Filter *filter );
    void reloadResizedImage( float resizedFactor );
    void initialize();
    bool checkCurrentFilter( bool showWarning = false );
    QString getSuggestedFileName( QString format );

    void clearGifHolder();
    void loadAnimatedGifHolder( QImageReader& qImageReader );

protected:

    void resizeEvent( QResizeEvent *event ) Q_DECL_OVERRIDE;

private Q_SLOTS:

    void createTest();
    void enableOpenGLFrontEnd();
    void loadImage();
    void saveImage();
    void exportSimilarityGraph();
    void loadOriginal();
    void enableFiltersFrame();
    void aboutDialog();
    void fillLabels( Image* image );

    void applyNearest();
    void applyBilinear();
    void applyHqx();
    void applyXbrZ();
    void applyXbr();
    void applyCRT();
    void applyScale2x();
    void applyEagle();
    void applySuperSaI2x();
    void createSimilarityGraph();
    void saveAnimatedGif();
    void applyVector();

    void finishFilter();
    void setProgress( int percentage );
    void finishSaveGif();
};

#endif // MAINWINDOW_H
