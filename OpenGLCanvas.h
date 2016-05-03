#ifndef OPENGLCANVAS_H
#define OPENGLCANVAS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QTime>

class QOpenGLShaderProgram;
class QOpenGLTexture;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

class OpenGLCanvas : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    OpenGLCanvas(QWidget *parent = 0);
    ~OpenGLCanvas();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void paintBasicExample();
    void paintBasicDeprecatedOpenGL();
    void initBasicExample();
    void initBasicDeprecatedOpenGL();
    void paintSphereRayTrace();
    void initSphereRayTrace();
    void loadShaderToyVariables();
    void initShaderToyCanvas();
    void paintShaderToyCanvas();

private:
    QMatrix4x4 m_modelView;
    QMatrix4x4 m_projection;
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer* _vbo;
    QOpenGLVertexArrayObject* _vao;
    QOpenGLTexture* _colorTexture;
    QOpenGLTexture* _sliceTexture;
    int _width;
    int _height;

    /* Mouse Events */
    QPoint _lastPos;

    QTime _time;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // OPENGLCANVAS_H
