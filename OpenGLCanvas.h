#ifndef OPENGLCANVAS_H
#define OPENGLCANVAS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QTime>
#include "DebugWindow.h"

class QOpenGLShaderProgram;
class QOpenGLTexture;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

class OpenGLCanvas :
    public QOpenGLWidget,
    protected QOpenGLFunctions
{
public:
    OpenGLCanvas( QWidget* parent = 0 );
    ~OpenGLCanvas();

    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();

    void paintBasicExample();
    void paintBasicDeprecatedOpenGL();

    void initBasicExample();
    void initBasicDeprecatedOpenGL();

    void loadShaderToyVariables();
    void initShaderToyCanvas();
    void paintShaderToyCanvas();

    void loadLibRetroVariables();
    void initLibRetroCanvas();
    void paintLibRetroCanvas();

    float getOpenGLVersion();
    void setTexture( QImage* image );
    void setPrograms( QString vertexShader, QString fragmentShader );

    //## DEBUG ##
    void setScaleFactor( double factor );
    void setMatrices( QMatrix4x4 mvp,
                      QMatrix4x4 proj,
                      QMatrix4x4 mv );

private:
    QMatrix4x4 m_modelView;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_MVPMatrix;
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer* _vbo;
    QOpenGLVertexArrayObject* _vao;
    QImage* _qImage;
    QOpenGLTexture* _texture;
    int _width;
    int _height;

    /* Mouse Events */
    QPoint _lastPos;
    float _zoom;
    QPoint _mousePosition;

    QTime _time;

    float _version;
    uchar* _texels;

    GLint _texLocation;

    QString _vertexShader;
    QString _fragmentShader;
    bool _isToLoadFromFile;

    //## DEBUG ##
    DebugWindow* _debugWindow;

protected:
    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void wheelEvent( QWheelEvent* event );
};

#endif // OPENGLCANVAS_H
