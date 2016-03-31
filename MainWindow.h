#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Image.h"
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
    SimilarityGraph* _similarityGraph;
    QAction* _openAct;
    QAction* _saveAct;
    QAction* _exitAct;
    QAction* _aboutAct;
    QMenu* _fileMenu;
    QMenu* _editMenu;
    QMenu* _helpMenu;

    void createActions();
    void createMenus();

    void fillQGraphicsView( QImage& qimage, unsigned int scaleFactor = 8 );

private Q_SLOTS:

    void createTest();
    void loadImage();
    void saveImage();
    void loadOriginal();
    void enableFiltersFrame();
    void aboutDialog();
    void fillLabels( Image image );
    void applyNearest();
    void applyHqx();
    void applyXbr();
    void applyXbrZ();
    void applyScale2x();
    void applyEagle();
    void createSimilarityGraph();
    void applyVector();
};

#endif // MAINWINDOW_H
