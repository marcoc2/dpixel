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
    _vbo( new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer ) ),
    _vao( new QOpenGLVertexArrayObject() ),
    _colorTexture( nullptr ),
    _width( parent->width() ),
    _height( parent->height() )
{
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


float OpenGLCanvas::getOpenGLVersion()
{
    return _version;
}


void OpenGLCanvas::initializeGL()
{
    initBasicExample();
    //initSphereRayTrace();
    //initShaderToyCanvas();
    QOpenGLFunctions glFuncs( QOpenGLContext::currentContext() );
    printf( "OpenGl information: VENDOR:       %s\n" , (const char*)glFuncs.glGetString(GL_VENDOR) );
    printf( "                    RENDERDER:    %s\n" , (const char*)glFuncs.glGetString(GL_RENDERER) );
    printf( "                    VERSION:      %s\n" , (const char*)glFuncs.glGetString(GL_VERSION ));
    printf( "                    GLSL VERSION: %s\n" , (const char*)glFuncs.glGetString(GL_SHADING_LANGUAGE_VERSION) );

    QString version = reinterpret_cast<const char*>( glFuncs.glGetString(GL_SHADING_LANGUAGE_VERSION) );

    int major = version.left(version.indexOf(".")).toInt();
    int minor = version.mid(version.indexOf(".") + 1, 1).toInt();
    _version = ( float ) major + ( ( float ) minor / 10 );

    if(!(major >= 4 && minor >= 0))
    {
        initBasicDeprecatedOpenGL();
    }
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
    //paintSphereRayTrace();
    if( _version >= 4.0f )
    {
        //paintShaderToyCanvas();
        paintBasicExample();
    }
    else
    {
        paintBasicDeprecatedOpenGL();
    }
    update();
}


void OpenGLCanvas::paintBasicExample()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    m_program->bind();
    m_program->setUniformValue( _texLocation, 1 );
    _vao->bind();
    _colorTexture->bind(0);

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

    _colorTexture->bind(0);
    _vao->release();
    m_program->release();
}


void OpenGLCanvas::paintBasicDeprecatedOpenGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    m_program->bind();
    //_vao->bind();

    //GLuint t;
    //glGenTextures(1, &t);
    //glBindTexture(GL_TEXTURE_2D, t);   // 2d texture (x and y size)
    //
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than
    //
    //glActiveTextureARB(GL_TEXTURE0_ARB);
    //glEnable(GL_TEXTURE_2D);
    //GLuint texLoc = m_program->uniformLocation("tex");
    //glUniform1i(texLoc,0);

    _colorTexture->bind();

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, _texels);

    //glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-0.8f,  0.8f, 0.0f);
        glTexCoord2f(-0.8f,  0.8f);
        glVertex3f( 0.8f,  0.8f, 0.0f);
        glTexCoord2f( 0.8f,  0.8f);
        glVertex3f( 0.8f, -0.8f, 0.0f);
        glTexCoord2f(0.8f, -0.8f);
        glVertex3f(-0.8f, -0.8f, 0.0f);
        glTexCoord2f(-0.8f, -0.8f);
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

    _texLocation = m_program->uniformLocation("tex");
    m_program->setUniformValue( _texLocation, 1 );

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

    GLfloat texCoords[ 8 ] = {
        -0.8, -0.8,
        -0.8, 0.8,
        0.8, -0.8,
        0.8, 0.8
    };

    _vao->create();
    _vao->bind();

    _vbo->create();
    _vbo->setUsagePattern( QOpenGLBuffer::StaticDraw );
    _vbo->bind();
    _vbo->allocate( 2 * 12 * sizeof( GLfloat ) + ( 1 * 8 * sizeof( GLfloat ) ) );
    _vbo->bind();
    _vbo->write( 0, plane, 12 * sizeof( GLfloat ) );
    _vbo->write( 12 * sizeof( GLfloat ), colors, 12 * sizeof( GLfloat ) );
    _vbo->write( 24 * sizeof( GLfloat ), texCoords, 8 * sizeof( GLfloat ) );

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

    glEnable(GL_COLOR_MATERIAL);

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


void OpenGLCanvas::setTexture( QImage& image )
{
    _colorTexture = new QOpenGLTexture( image.mirrored().mirrored().convertToFormat(QImage::Format_RGBA8888) );
    _colorTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _colorTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _colorTexture->setWrapMode(QOpenGLTexture::DirectionS,
                              QOpenGLTexture::ClampToEdge);
    _colorTexture->setWrapMode(QOpenGLTexture::DirectionT,
                              QOpenGLTexture::ClampToEdge);
}
