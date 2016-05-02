#ifndef CHECKUPSCALEWINDOW_H
#define CHECKUPSCALEWINDOW_H

#include <QDialog>

namespace Ui
{
class CheckUpscaleWindow;
}
class QGraphicsScene;
class Image;

class CheckUpscaleWindow : public QDialog
{
    Q_OBJECT
public:
    CheckUpscaleWindow( QWidget *parent, Image* image );
    ~CheckUpscaleWindow();

private:

    void drawGridOnImage( int resizedFactor );

    Ui::CheckUpscaleWindow* _ui;
    Image* _copiedImage;
    Image* _resizedImage;
    QGraphicsScene* _imageScene;
    QGraphicsScene* _fitGraphicsScene;

private Q_SLOTS:

    void okButtonCallback();
    void downscaleImage();
};

#endif // CHECKUPSCALEWINDOW_H
