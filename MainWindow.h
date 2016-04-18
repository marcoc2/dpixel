#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Image.h"
#include "Filters/Filter.h"
#include <QMainWindow>
#include <QRadioButton>
#include <QGraphicsView>

class SimilarityGraph;

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

private:

    Ui::MainWindow* _ui;
    Image* _inputImage;
    Image* _outputImage;
    QString _currentFileName;
    SimilarityGraph* _similarityGraph;
    QAction* _openAct;
    QAction* _saveAct;
    QAction* _exitAct;
    QAction* _aboutAct;
    QMenu* _fileMenu;
    QMenu* _editMenu;
    QMenu* _helpMenu;

    QGraphicsScene* _resultScene;
    QGraphicsScene* _originalScene;
    QGraphicsScene* _graphScene;

    bool _isAnimatedGif;


    void createActions();
    void createMenus();

    void fillQGraphicsView( QImage& qimage, u_int scaleFactor = 8 );
    void fillQGraphicsViewOriginal( QImage& qimage, u_int scaleFactor = 8 );
    void applyAndShowOutputImage( Filter& filter );
    void reloadResizedImage( int resizedFactor );
    void initialize();

protected:

    void resizeEvent( QResizeEvent *event ) Q_DECL_OVERRIDE;

private Q_SLOTS:

    void createTest();
    void loadImage();
    void saveImage();
    void loadOriginal();
    void enableFiltersFrame();
    void aboutDialog();
    void fillLabels( Image* image );

    void applyNearest();
    void applyHqx();
    void applyXbr();
    void applyXbrZ();
    void applyCRT();
    void applyScale2x();
    void applyEagle();
    void createSimilarityGraph();
    void saveAnimatedGif();
    void applyVector();
};

#endif // MAINWINDOW_H
