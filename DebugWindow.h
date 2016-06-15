#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QDialog>

class OpenGLCanvas;
namespace Ui
{
class DebugWindow;
}

class DebugWindow : public QDialog
{
    Q_OBJECT
public:
    DebugWindow( QWidget *parent );
    ~DebugWindow();

void setData( QMatrix4x4 mvp, QMatrix4x4 projection,
              QMatrix4x4 modelview, float scaleFactor );

private:

    Ui::DebugWindow* _ui;
    OpenGLCanvas* _canvas;

private Q_SLOTS:

    void changeScaleFactor( double factor );
    void changeMatrices( double value );
};

#endif // DEBUGWINDOW_H
