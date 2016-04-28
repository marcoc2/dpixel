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

private:

    Ui::CheckUpscaleWindow* _ui;
    Image* _image;
    Image* _resizedImage;
    QGraphicsScene* _imageScene;

private Q_SLOTS:

    void okButtonCallback();
    void downscaleImage();
};

#endif // CHECKUPSCALEWINDOW_H
