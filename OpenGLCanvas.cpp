#include "OpenGLCanvas.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLPixelTransferOptions>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QMouseEvent>

OpenGLCanvas::OpenGLCanvas( QWidget* parent ) :
    QOpenGLWidget( parent ),
    m_program( 0 ),
    _width( parent->width() ),
    _height( parent->height() )
{
    _vbo = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
    _vao = new QOpenGLVertexArrayObject();
    _time = QTime( 0, 0, 0, 0 );
    QSurfaceFormat format;
    format.setDepthBufferSize( 24 );
    setFormat( format );
}


OpenGLCanvas::~OpenGLCanvas()
{
    makeCurrent();

    doneCurrent();
}


void OpenGLCanvas::initializeGL()
{
    //initBasicExample();
    //initBasicDeprecatedOpenGL();
    //initSphereRayTrace();
    initShaderToyCanvas();
}


void OpenGLCanvas::mousePressEvent( QMouseEvent* event )
{
    _lastPos = event->pos();
}


void OpenGLCanvas::mouseMoveEvent( QMouseEvent* event )
{
//    int dx = event->x() - lastPos.x();
//    int dy = event->y() - lastPos.y();

    _lastPos = event->pos();
}


void OpenGLCanvas::resizeGL( int w, int h )
{
    m_projection.setToIdentity();
    m_projection.perspective( 45.0f, w / float( h ), 0.1f, 1000.0f );
    _width = w;
    _height = h;
}


void OpenGLCanvas::paintGL()
{
    //paintBasicExample();
    //paintBasicDeprecatedOpenGL();
    //paintSphereRayTrace();
    paintShaderToyCanvas();
    update();
}


void OpenGLCanvas::paintBasicExample()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    m_program->bind();
    _vao->bind();

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    _vao->release();
    m_program->release();
}


void OpenGLCanvas::paintBasicDeprecatedOpenGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    m_program->bind();
    //_vao->bind();

    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-0.8f,  0.8f, 0.0f);
        glVertex3f( 0.8f,  0.8f, 0.0f);
        glVertex3f( 0.8f, -0.8f, 0.0f);
        glVertex3f(-0.8f, -0.8f, 0.0f);
    glEnd();

    //_vao->release();
    m_program->release();
}


void OpenGLCanvas::initBasicExample()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram();

    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex,
                                        "../Shaders/basicVertex.vert" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment,
                                        "../Shaders/basicFragment.frag" );
    if( !m_program->link() )
    {
        qWarning( "Failed to compile and link shader program" );
        qWarning( "Shader program log:" );
        qWarning() << m_program->log();

        delete m_program;
        m_program = 0;
    }

    GLfloat plane[ 12 ] = {
        -0.8, -0.8, 0.0,
        -0.8, 0.8, 0.0,
        0.8, -0.8, 0.0,
        0.8, 0.8, 0.0
    };

    GLfloat colors[ 12 ] = {
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0
    };

    _vao->create();
    _vao->bind();

    _vbo->create();
    _vbo->setUsagePattern( QOpenGLBuffer::StaticDraw );
    _vbo->bind();
    _vbo->allocate( 2 * 12 * sizeof( GLfloat ) );
    _vbo->bind();
    _vbo->write( 0, plane, 12 * sizeof( GLfloat ) );
    _vbo->write( 12 * sizeof( GLfloat ), colors, 12 * sizeof( GLfloat ) );

    m_program->setAttributeBuffer( "vertex_position", GL_FLOAT, 0, 3, 0 );
    m_program->enableAttributeArray( "vertex_position" );

    m_program->setAttributeBuffer( "vertex_color", GL_FLOAT, 12 * sizeof( GLfloat ), 3, 0 );
    m_program->enableAttributeArray( "vertex_color" );

    _vao->release();
    m_program->release();
}


void OpenGLCanvas::initBasicDeprecatedOpenGL()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram();

    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex,
                                        "../Shaders/basicVertex.vert" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment,
                                        "../Shaders/basicFragment.frag" );
    if( !m_program->link() )
    {
        qWarning( "Failed to compile and link shader program" );
        qWarning( "Shader program log:" );
        qWarning() << m_program->log();

        delete m_program;
        m_program = 0;
    }

    glBegin(GL_TRIANGLES);
        glVertex3f(-0.8, -0.8, 0.0);
        glVertex3f(-0.8, 0.8, 0.0);
        glVertex3f(0.8, -0.8, 0.0);
        glVertex3f(0.8, 0.8, 0.0);
    glEnd();

    m_program->release();
}


void OpenGLCanvas::paintSphereRayTrace()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_POLYGON_BIT | GL_POINT_BIT );
    glPushClientAttrib( GL_CLIENT_ALL_ATTRIB_BITS );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_COLOR_MATERIAL );

    glMatrixMode( GL_MODELVIEW );

    glEnable( GL_VERTEX_PROGRAM_POINT_SIZE_ARB );
    glEnable( GL_POINT_SPRITE_ARB );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_FOG_COORD_ARRAY );

    // passa tamanho da tela
    QVector2D size( ( float ) _width, ( float ) _height );
    m_program->setUniformValueArray( "screenSize", &size, 1 );

    m_program->bind();
    _vao->bind();

    glDrawArrays( GL_POINTS, 0, 4 );

    _vao->release();
    m_program->release();
}


void OpenGLCanvas::initSphereRayTrace()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram();

    m_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "/home/marco/Dropbox/Programação/LSD-Shaders/LSD-Shaders/Shaders/basicVertex.vert" );
    m_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "/home/marco/Dropbox/Programação/LSD-Shaders/LSD-Shaders/Shaders/basicPragment.frag" );
    if( !m_program->link() )
    {
        qWarning( "Failed to compile and link shader program" );
        qWarning( "Shader program log:" );
        qWarning() << m_program->log();

        delete m_program;
        m_program = 0;
    }

    GLfloat spheres[ 12 ] = {
        -0.5, -0.5, -0.5,
        -0.5, 0.5, 0.0,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.0
    };

    _vao->create();
    _vao->bind();

    _vbo->create();
    _vbo->setUsagePattern( QOpenGLBuffer::StaticDraw );
    _vbo->bind();
    _vbo->allocate( 12 * sizeof( GLfloat ) );
    _vbo->bind();
    _vbo->write( 0, spheres, 12 * sizeof( GLfloat ) );

    int loc = m_program->attributeLocation( "sphereOrigin" );
    m_program->setAttributeBuffer( loc, GL_FLOAT, 0, 3, 0 );
    m_program->enableAttributeArray( loc );

//    unsigned int vbo;
//    QOpenGLFunctions::glGenBuffers(1, &vbo);
//    QOpenGLFunctions::glBindBuffer( GL_ARRAY_BUFFER, vbo);
//    QOpenGLFunctions::glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 4 * 3, &spheres, GL_STATIC_DRAW );

    _vao->release();
    m_program->release();
}


void OpenGLCanvas::loadShaderToyVariables()
{
    /*
    uniform vec3      iResolution;           // viewport resolution (in pixels)
    uniform float     iGlobalTime;           // shader playback time (in seconds)
    uniform float     iChannelTime[4];       // channel playback time (in seconds)
    uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
    uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
    uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
    uniform vec4      iDate;                 // (year, month, day, time in seconds)
    uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
    */

    const QVector3D resolution = QVector3D( ( float ) _width, ( float ) _height, 0.0 );
    m_program->setUniformValue( "iResolution", resolution );

    const QVector4D mouse = QVector4D( ( float ) _lastPos.x(), ( float ) _lastPos.y(), 0.0, 0.0 );
    m_program->setUniformValue( "iMouse", mouse );

    m_program->setUniformValue( "iGlobalTime", ( float ) _time.currentTime().msecsSinceStartOfDay() / 1000 );
    _time.addMSecs( 1 );
}


void OpenGLCanvas::paintShaderToyCanvas()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    m_program->bind();
    _vao->bind();

    loadShaderToyVariables();

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    _vao->release();
    m_program->release();
}


void OpenGLCanvas::initShaderToyCanvas()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram();

    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex,
                                        "../Shaders/basicVertex.vert" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment,
                                        "../Shaders/lsd_ufos.frag" );
    if( !m_program->link() )
    {
        qWarning( "Failed to compile and link shader program" );
        qWarning( "Shader program log:" );
        qWarning() << m_program->log();

        delete m_program;
        m_program = 0;
    }

    GLfloat plane[ 12 ] = {
        -0.95, -0.95, 0.0,
        -0.95, 0.95, 0.0,
        0.95, -0.95, 0.0,
        0.95, 0.95, 0.0
    };

    _vao->create();
    _vao->bind();

    _vbo->create();
    _vbo->setUsagePattern( QOpenGLBuffer::StaticDraw );
    _vbo->bind();
    _vbo->allocate( 12 * sizeof( GLfloat ) );
    _vbo->bind();
    _vbo->write( 0, plane, 12 * sizeof( GLfloat ) );

    m_program->setAttributeBuffer( "vertex_position", GL_FLOAT, 0, 3, 0 );
    m_program->enableAttributeArray( "vertex_position" );

    _vao->release();
    m_program->release();
}


