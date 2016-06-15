#include "DebugWindow.h"
#include "ui_DebugWindow.h"
#include <QMatrix4x4>
#include "OpenGLCanvas.h"

DebugWindow::DebugWindow( QWidget* parent ) :
    QDialog( parent ),
    _ui( new Ui::DebugWindow ),
    _canvas( static_cast< OpenGLCanvas* >( parent ) )
{
    _ui->setupUi( this );

    connect( _ui->scale, SIGNAL( valueChanged( double ) ), this, SLOT( changeScaleFactor( double ) ) );

    show();
}


DebugWindow::~DebugWindow()
{
    delete _ui;
}


void DebugWindow::setData( QMatrix4x4 mvp,
                           QMatrix4x4 projection,
                           QMatrix4x4 modelview,
                           float scaleFactor )
{
    _ui->scale->setValue( scaleFactor );

    _ui->mvp0->setValue(  mvp(0,0) );
    _ui->mvp1->setValue(  mvp(0,1) );
    _ui->mvp2->setValue(  mvp(0,3) );
    _ui->mvp3->setValue(  mvp(0,4) );
    _ui->mvp4->setValue(  mvp(1,0) );
    _ui->mvp5->setValue(  mvp(1,1) );
    _ui->mvp6->setValue(  mvp(1,2) );
    _ui->mvp7->setValue(  mvp(1,3) );
    _ui->mvp8->setValue(  mvp(2,0) );
    _ui->mvp9->setValue(  mvp(2,1) );
    _ui->mvp10->setValue( mvp(2,2) );
    _ui->mvp11->setValue( mvp(2,3) );
    _ui->mvp12->setValue( mvp(3,0) );
    _ui->mvp13->setValue( mvp(3,1) );
    _ui->mvp14->setValue( mvp(3,2) );
    _ui->mvp15->setValue( mvp(3,3) );

    _ui->proj0->setValue(  projection(0,0) );
    _ui->proj1->setValue(  projection(0,1) );
    _ui->proj2->setValue(  projection(0,3) );
    _ui->proj3->setValue(  projection(0,4) );
    _ui->proj4->setValue(  projection(1,0) );
    _ui->proj5->setValue(  projection(1,1) );
    _ui->proj6->setValue(  projection(1,2) );
    _ui->proj7->setValue(  projection(1,3) );
    _ui->proj8->setValue(  projection(2,0) );
    _ui->proj9->setValue(  projection(2,1) );
    _ui->proj10->setValue( projection(2,2) );
    _ui->proj11->setValue( projection(2,3) );
    _ui->proj12->setValue( projection(3,0) );
    _ui->proj13->setValue( projection(3,1) );
    _ui->proj14->setValue( projection(3,2) );
    _ui->proj15->setValue( projection(3,3) );

    _ui->mv0->setValue(  modelview(0,0) );
    _ui->mv1->setValue(  modelview(0,1) );
    _ui->mv2->setValue(  modelview(0,3) );
    _ui->mv3->setValue(  modelview(0,4) );
    _ui->mv4->setValue(  modelview(1,0) );
    _ui->mv5->setValue(  modelview(1,1) );
    _ui->mv6->setValue(  modelview(1,2) );
    _ui->mv7->setValue(  modelview(1,3) );
    _ui->mv8->setValue(  modelview(2,0) );
    _ui->mv9->setValue(  modelview(2,1) );
    _ui->mv10->setValue( modelview(2,2) );
    _ui->mv11->setValue( modelview(2,3) );
    _ui->mv12->setValue( modelview(3,0) );
    _ui->mv13->setValue( modelview(3,1) );
    _ui->mv14->setValue( modelview(3,2) );
    _ui->mv15->setValue( modelview(3,3) );
}


void DebugWindow::changeScaleFactor( double factor )
{
    _canvas->setScaleFactor( factor );
}


void DebugWindow::changeMatrices( double value )
{
    QMatrix4x4 mvp, proj, mv;
    mvp(0,0) = _ui->mvp0->value();
    mvp(0,1) = _ui->mvp1->value();
    mvp(0,3) = _ui->mvp2->value();
    mvp(0,4) = _ui->mvp3->value();
    mvp(1,0) = _ui->mvp4->value();
    mvp(1,1) = _ui->mvp5->value();
    mvp(1,2) = _ui->mvp6->value();
    mvp(1,3) = _ui->mvp7->value();
    mvp(2,0) = _ui->mvp8->value();
    mvp(2,1) = _ui->mvp9->value();
    mvp(2,2) = _ui->mvp10->value();
    mvp(2,3) = _ui->mvp11->value();
    mvp(3,0) = _ui->mvp12->value();
    mvp(3,1) = _ui->mvp13->value();
    mvp(3,2) = _ui->mvp14->value();
    mvp(3,3) = _ui->mvp15->value();

    proj(0,0) = _ui->proj0->value(  );
    proj(0,1) = _ui->proj1->value(  );
    proj(0,3) = _ui->proj2->value(  );
    proj(0,4) = _ui->proj3->value(  );
    proj(1,0) = _ui->proj4->value(  );
    proj(1,1) = _ui->proj5->value(  );
    proj(1,2) = _ui->proj6->value(  );
    proj(1,3) = _ui->proj7->value(  );
    proj(2,0) = _ui->proj8->value(  );
    proj(2,1) = _ui->proj9->value(  );
    proj(2,2) = _ui->proj10->value( );
    proj(2,3) = _ui->proj11->value( );
    proj(3,0) = _ui->proj12->value( );
    proj(3,1) = _ui->proj13->value( );
    proj(3,2) = _ui->proj14->value( );
    proj(3,3) = _ui->proj15->value( );

    mv(0,0) = _ui->mv0->value(   );
    mv(0,1) = _ui->mv1->value(   );
    mv(0,3) = _ui->mv2->value(   );
    mv(0,4) = _ui->mv3->value(   );
    mv(1,0) = _ui->mv4->value(   );
    mv(1,1) = _ui->mv5->value(   );
    mv(1,2) = _ui->mv6->value(   );
    mv(1,3) = _ui->mv7->value(   );
    mv(2,0) = _ui->mv8->value(   );
    mv(2,1) = _ui->mv9->value(   );
    mv(2,2) = _ui->mv10->value(  );
    mv(2,3) = _ui->mv11->value(  );
    mv(3,0) = _ui->mv12->value(  );
    mv(3,1) = _ui->mv13->value(  );
    mv(3,2) = _ui->mv14->value(  );
    mv(3,3) = _ui->mv15->value(  );

    _canvas->setMatrices( mvp, proj, mv );
}
