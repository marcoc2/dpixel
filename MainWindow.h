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
#include <QFileSystemWatcher>

class SimilarityGraph;
class Filter;
class GifSaver;
class QTreeWidgetItem;

namespace Ui
{
class MainWindow;
}

class MainWindow :
    public QMainWindow
{
    Q_OBJECT

    enum class FrontEnd
    {
        CPU_IMAGE,
        OPENGL
    };

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
    QFileSystemWatcher _fileWatcher;

    QGraphicsScene* _resultScene;
    QGraphicsScene* _originalScene;
    QGraphicsScene* _graphScene;

    QPointF _resultSceneRatio;
    QPointF _originalSceneRatio;
    QPointF _graphSceneRatio;

    GifSaver* _gifSaver;

    bool _isAnimatedGif;

    FrontEnd _frontEndEnabled;

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
    void listFolderItems( QString folder, QTreeWidgetItem* item );
    void dropEvent( QDropEvent* event );
    void dragEnterEvent( QDragEnterEvent* event );
    void setOpenGLCanvasData();

protected:

    void resizeEvent( QResizeEvent *event ) Q_DECL_OVERRIDE;

private Q_SLOTS:

    void createTest();
    void changeFrontEnd( int FrontEnd );
    void changeShader( QTreeWidgetItem *item, int column );
    void updateShaderFile( const QString& path );
    void shaderFileChangedCallback( const QString &path );
    void loadImage( QString path = "");
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
    void applyBeads();
    void applyScale2x();
    void applyEagle();
    void applySuperSaI2x();
    void applyGIFDelay( int delay );
    void createSimilarityGraph();
    void saveAnimatedGif();
    void applyVector();

    void finishFilter();
    void setProgress( int percentage );
    void finishSaveGif();
};

#endif // MAINWINDOW_H
