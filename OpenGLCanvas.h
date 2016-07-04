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

    void paintBasicDeprecatedOpenGL();
    void initBasicDeprecatedOpenGL();

    void loadLibRetroVariables();
    void initLibRetroCanvas();
    void paintLibRetroCanvas();

    float getOpenGLVersion();
    void setTexture( QImage* image );
    void setShaderSource( QString vertexShader, QString fragmentShader );
    QImage exportFrameBuffer();
    void setGifVector( std::vector< QImage* > animatedGif );
    void stopRenderLoop( bool state );
    void setAnimationDelay( int delay );

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
    std::vector< QImage* > _animatedGif;
    QOpenGLTexture* _texture;
    int _width;
    int _height;

    /* Mouse Events */
    QPoint _lastPostition;
    QPoint _currentPosition;
    QPoint _mousePosition;

    float _scaleFactor;

    QTime _time;
    int _spriteTime;
    int _spriteIndex;
    int _spriteDelay;

    float _version;
    uchar* _texels;

    GLint _texLocation;

    QString _vertexShader;
    QString _fragmentShader;

    bool _isToLoadFromFile;
    bool _stopRender;

    //## DEBUG ##
    DebugWindow* _debugWindow;

protected:
    void mousePressEvent( QMouseEvent* event );
    void mouseDoubleClickEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void wheelEvent( QWheelEvent* event );
};

#endif // OPENGLCANVAS_H
