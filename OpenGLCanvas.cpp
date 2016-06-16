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
    _qImage( nullptr ),
    _texture( nullptr ),
    _width( parent->width() ),
    _height( parent->height() ),
    _lastPostition( 0.0f, 0.0f ),
    _currentPosition( 0.0f, 0.0f ),
    _zoom( 1.0 ),
    _mousePosition( 0 ,0 ),
    _vertexShader( "../Shaders/crt-hyllian_vert.glsl" ),
    _fragmentShader( "../Shaders/crt-hyllian_frag.glsl" ),
    _isToLoadFromFile( true ),
    _debugWindow( nullptr )//new DebugWindow( this ) )
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

    delete _vbo;
    delete _vao;
}


float OpenGLCanvas::getOpenGLVersion()
{
    return _version;
}


void OpenGLCanvas::initializeGL()
{
    //initBasicExample();
    //initSphereRayTrace();
    //initShaderToyCanvas();
    initLibRetroCanvas();
    QOpenGLFunctions glFuncs( QOpenGLContext::currentContext() );
    printf( "OpenGl information: VENDOR:       %s\n", ( const char* )glFuncs.glGetString( GL_VENDOR ) );
    printf( "                    RENDERDER:    %s\n", ( const char* )glFuncs.glGetString( GL_RENDERER ) );
    printf( "                    VERSION:      %s\n", ( const char* )glFuncs.glGetString( GL_VERSION ) );
    printf( "                    GLSL VERSION: %s\n",
            ( const char* )glFuncs.glGetString( GL_SHADING_LANGUAGE_VERSION ) );

    QString version = reinterpret_cast< const char* >( glFuncs.glGetString( GL_SHADING_LANGUAGE_VERSION ) );

    int major = version.left( version.indexOf( "." ) ).toInt();
    int minor = version.mid( version.indexOf( "." ) + 1, 1 ).toInt();
    _version = ( float ) major + ( ( float ) minor / 10 );

    if( !( major >= 4 && minor >= 0 ) )
    {
        initBasicDeprecatedOpenGL();
    }
}


void OpenGLCanvas::mousePressEvent( QMouseEvent* event )
{
    _lastPostition = event->pos();
}


void OpenGLCanvas::mouseDoubleClickEvent( QMouseEvent* event )
{
    _zoom = 1.0;
    _currentPosition = QPoint( 0.0f, 0.0f );
}


void OpenGLCanvas::mouseMoveEvent( QMouseEvent* event )
{
    QPoint delta;
    delta.setX( event->x() - _lastPostition.x() );
    delta.setY( event->y() - _lastPostition.y() );

    _lastPostition = event->pos();
    _mousePosition = event->pos();

    _currentPosition += delta;

    //printf( "mouse position: %d, %d\n", event->pos().x(), event->pos().y() );
}


void OpenGLCanvas::wheelEvent( QWheelEvent* event )
{
    _zoom  += event->angleDelta().y() / (8.0 * 60);
}

void OpenGLCanvas::resizeGL( int w, int h )
{
    m_projection.setToIdentity();
    m_projection.perspective( 45.0f, w / float( h ), 0.1f, 1.0f );

    //m_modelView.lookAt( QVector3D( 0.5, 0.5, 1.0 ), QVector3D( 0.5, 0.5, 0.0 ), QVector3D( 0.0, 1.0, 0.0 ) );
    //m_MVPMatrix = m_projection * m_modelView;

    _width = w;
    _height = h;
}


void OpenGLCanvas::paintGL()
{
    if( _version >= 4.0f )
    {
        paintLibRetroCanvas();
        //paintShaderToyCanvas();
        //paintBasicExample();
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
    m_program->setUniformValue( _texLocation, 0 );
    _vao->bind();

    if( _texture && _texture->isCreated() )
    {
        _texture->bind();
    }

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

    if( _texture && _texture->isCreated() )
    {
        _texture->release();
    }
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

    //_colorTexture->bind();

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, _texels);

    //glColor3f(1.0f,0.0f,0.0f);
    //glBegin(GL_QUADS);
    //    glVertex3f(-0.8f,  0.8f, 0.0f);
    //    glTexCoord2f(-0.8f,  0.8f);
    //    glVertex3f( 0.8f,  0.8f, 0.0f);
    //    glTexCoord2f( 0.8f,  0.8f);
    //    glVertex3f( 0.8f, -0.8f, 0.0f);
    //    glTexCoord2f(0.8f, -0.8f);
    //    glVertex3f(-0.8f, -0.8f, 0.0f);
    //    glTexCoord2f(-0.8f, -0.8f);
    //glEnd();

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

    _texture = new QOpenGLTexture( _qImage->mirrored().convertToFormat( QImage::Format_RGBA8888 ) );

    _texture->setMinificationFilter( QOpenGLTexture::LinearMipMapLinear );
    _texture->setMagnificationFilter( QOpenGLTexture::Nearest );
    _texture->setWrapMode( QOpenGLTexture::DirectionS,
                           QOpenGLTexture::ClampToEdge );
    _texture->setWrapMode( QOpenGLTexture::DirectionT,
                           QOpenGLTexture::ClampToEdge );

    _texLocation = m_program->uniformLocation( "tex" );
    m_program->setUniformValue( _texLocation, 0 );

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
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0
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

    m_program->setAttributeBuffer( "vertex_uv", GL_FLOAT, 24 * sizeof( GLfloat ), 2, 0 );
    m_program->enableAttributeArray( "vertex_uv" );

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

    glEnable( GL_COLOR_MATERIAL );

    //glBegin(GL_TRIANGLES);
    //    glVertex3f(-0.8, -0.8, 0.0);
    //    glVertex3f(-0.8, 0.8, 0.0);
    //    glVertex3f(0.8, -0.8, 0.0);
    //    glVertex3f(0.8, 0.8, 0.0);
    //glEnd();

    m_program->release();
}


void OpenGLCanvas::loadLibRetroVariables()
{
    /*
        gl_glsl_get_uniform(glsl, prog, "MVPMatrix");
        gl_glsl_get_attrib(glsl, prog, "TexCoord");
        gl_glsl_get_attrib(glsl, prog, "VertexCoord");
        gl_glsl_get_attrib(glsl, prog, "Color");
        gl_glsl_get_attrib(glsl, prog, "LUTTexCoord");

        gl_glsl_get_uniform(glsl, prog, "InputSize");
        gl_glsl_get_uniform(glsl, prog, "OutputSize");
        gl_glsl_get_uniform(glsl, prog, "TextureSize");

        gl_glsl_get_uniform(glsl, prog, "FrameCount");
        gl_glsl_get_uniform(glsl, prog, "FrameDirection");
    */

    m_program->setUniformValue( _texLocation, 0 );

    //QMatrix4x4 modelView;
    //modelView.lookAt( QVector3D( 0.5, 0.5, 1.0 ), QVector3D( 0.5, 0.5, 0.0 ), QVector3D( 0.0, 1.0, 0.0 ) );
    //QMatrix4x4 modelViewProjection = m_projection * modelView;

    //printf( "mouse position: %d, %d\n", _currentPosition.x(), _currentPosition.y() );

    QMatrix4x4 modelViewProjection;
    modelViewProjection.setToIdentity();
    modelViewProjection.translate( ( float ) _currentPosition.x()/1000,( float ) - _currentPosition.y()/1000 );
    modelViewProjection.scale( _zoom );

    //_debugWindow->setData( m_MVPMatrix, m_projection, modelView, _zoom );

    //const QVector3D resolution = QVector3D( ( float ) _width, ( float ) _height, 0.0 );
    m_program->setUniformValue( "MVPMatrix", modelViewProjection );

    // 1 or -1
    m_program->setUniformValue( "FrameDirection", 1 );

    m_program->setUniformValue( "FrameCount", 30 );

    const QVector2D outputSize = QVector2D( ( float ) _width, ( float ) _height );
    m_program->setUniformValue( "OutputSize", outputSize );

    const QVector2D textureSize = QVector2D( ( float ) _texture->width(), ( float ) _texture->height() );
    m_program->setUniformValue( "TextureSize", textureSize );

    const QVector2D inputSize = QVector2D( ( float ) _texture->width(), ( float ) _texture->height() );
    m_program->setUniformValue( "InputSize", inputSize );
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

    const QVector4D mouse = QVector4D( ( float ) _lastPostition.x(), ( float ) _lastPostition.y(), 0.0, 0.0 );
    m_program->setUniformValue( "iMouse", mouse );

    m_program->setUniformValue( "iGlobalTime", ( float ) _time.currentTime().msecsSinceStartOfDay() / 1000 );
    _time.addMSecs( 1 );
}


void OpenGLCanvas::paintLibRetroCanvas()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    m_program->bind();

    loadLibRetroVariables();

    _vao->bind();

    if( _texture && _texture->isCreated() )
    {
        _texture->bind();
    }

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    if( _texture && _texture->isCreated() )
    {
        _texture->release();
    }

    _vao->release();
    m_program->release();
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


void OpenGLCanvas::initLibRetroCanvas()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram();

    if( _isToLoadFromFile )
    {
        m_program->addShaderFromSourceFile( QOpenGLShader::Vertex,
                                            _vertexShader );
        m_program->addShaderFromSourceFile( QOpenGLShader::Fragment,
                                            _fragmentShader );
    }
    else
    {
        m_program->addShaderFromSourceCode( QOpenGLShader::Vertex,
                                            _vertexShader );
        m_program->addShaderFromSourceCode( QOpenGLShader::Fragment,
                                            _fragmentShader );
    }
    if( !m_program->link() )
    {
        qWarning( "Failed to compile and link shader program" );
        qWarning( "Shader program log:" );
        qWarning() << m_program->log();

        delete m_program;
        m_program = 0;
    }

    _texture = new QOpenGLTexture( _qImage->mirrored().convertToFormat( QImage::Format_RGBA8888 ) );

    _texture->setMinificationFilter( QOpenGLTexture::LinearMipMapLinear );
    _texture->setMagnificationFilter( QOpenGLTexture::Nearest );
    _texture->setWrapMode( QOpenGLTexture::DirectionS,
                           QOpenGLTexture::ClampToEdge );
    _texture->setWrapMode( QOpenGLTexture::DirectionT,
                           QOpenGLTexture::ClampToEdge );

    _texLocation = m_program->uniformLocation( "Texture" );
    m_program->setUniformValue( _texLocation, 0 );

    GLfloat plane[ 16 ] = {
        -1.0, -1.0, 0.0, 1.0,
        -1.0, 1.0, 0.0, 1.0,
        1.0, -1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0
    };

    //for( int i = 0; i < 16; i++ )
    //{
    //    plane[i] = plane[i] / 3.0;
    //}

    GLfloat colors[ 16 ] = {
        0.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 0.0, 0.0
    };

    GLfloat texCoords[ 16 ] = {
        0.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 0.0, 0.0
    };

    _vao->create();
    _vao->bind();

    _vbo->create();
    _vbo->setUsagePattern( QOpenGLBuffer::StaticDraw );
    _vbo->bind();
    _vbo->allocate( 3 * 16 * sizeof( GLfloat ) );
    _vbo->bind();
    _vbo->write( 0, plane, 16 * sizeof( GLfloat ) );
    _vbo->write( 16 * sizeof( GLfloat ), colors, 16 * sizeof( GLfloat ) );
    _vbo->write( 32 * sizeof( GLfloat ), texCoords, 16 * sizeof( GLfloat ) );

    m_program->setAttributeBuffer( "VertexCoord", GL_FLOAT, 0, 4, 0 );
    m_program->enableAttributeArray( "VertexCoord" );

    m_program->setAttributeBuffer( "COLOR", GL_FLOAT, 16 * sizeof( GLfloat ), 4, 0 );
    m_program->enableAttributeArray( "COLOR" );

    m_program->setAttributeBuffer( "TexCoord", GL_FLOAT, 32 * sizeof( GLfloat ), 4, 0 );
    m_program->enableAttributeArray( "TexCoord" );

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


void OpenGLCanvas::setTexture( QImage* image )
{
    _qImage = image;
}


void OpenGLCanvas::setPrograms( QString vertexShader, QString fragmentShader )
{
    _vertexShader = vertexShader;
    _fragmentShader = fragmentShader;

    _isToLoadFromFile = false;

    initializeGL();
}


void OpenGLCanvas::setScaleFactor( double factor )
{
    _zoom = factor;
}


void OpenGLCanvas::setMatrices( QMatrix4x4 mvp,
                                QMatrix4x4 proj,
                                QMatrix4x4 mv )
{
    m_MVPMatrix = mvp;
    m_projection = proj;
    m_modelView = mv;
}


QImage OpenGLCanvas::exportFrameBuffer()
{
    return grabFramebuffer();
}
