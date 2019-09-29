#include <QFile>
#include <QString>
#include <QTransform>
#include <QtGui>
#include <QKeyEvent>
#include <iostream>
#include "glwidget.h"

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
     QWidget::setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

GLWidget::~GLWidget() {}


void GLWidget::loadFaces(const QString &path) {
  QFile json_file(path);
  if (!json_file.open(QIODevice::ReadOnly)) {
    qWarning("Failed to open file");
    exit(-1);
  }
  QByteArray json_data = json_file.readAll();
  QJsonDocument json_document(QJsonDocument::fromJson(json_data));
  face_collection.fromJson(json_document.array());

  face_collection.init=true;
  // update the display inside the widget
  paintGL();
}

void GLWidget::initializeGL() {

    static const float init_zoom = 0.0294083;
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    setMouseTracking(true);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(init_zoom, init_zoom, init_zoom);
}

void GLWidget::paintGL() {
  QSize viewport_size = size();
  glViewport(0, 0, viewport_size.width(), viewport_size.height());

  /*
   *  Config Projection Matrix  GL_PROJECTION
   */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                 // Reset the model-view matrix
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers



  /*
   *  Config Model View Matrix  GL_MODELVIEW
   */
  glMatrixMode(GL_MODELVIEW);
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  glLoadIdentity();
  /*Rotation*/
  glRotatef(rotation_angle,rotation.y(),rotation.x(),0.0f);
  rotation_angle = 0;
  /*Translation*/
  glTranslatef(translation.x(),translation.y(),0.0f);
  translation.rx()=0;
  translation.ry()=0;
  /*Scale*/
  glScalef(zoomScale, zoomScale, zoomScale);
  zoomScale = 1;

  glMultMatrixf(m);
  glGetFloatv(GL_MODELVIEW_MATRIX, m);

    /* Print the matrix */
/*
  for(int i=0;i<16;i++)
  {
      cout<<m[i]<<" ";
  }
  cout<<endl;
*/

  //*********** Draw the model//
  // Define vertices in counter-clockwise (CCW) order with normal pointing out
  if(face_collection.init==true)
  {

      /*glDepthMask(GL_FALSE);*/
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glBegin(GL_QUADS);

      for (int face_index=0;face_index<face_collection.faces.size();face_index++)
      {
          Face face = face_collection.faces[face_index];
          float color = face_index/(float)face_collection.faces.size() - 0.1;//face.c;//float)// //
          glColor4f(color, color, color, _alphaNew);
          glVertex3f( face.vertices[0].x(), face.vertices[0].y(), face.vertices[0].z());
          glVertex3f( face.vertices[1].x(), face.vertices[1].y(), face.vertices[1].z());
          glVertex3f( face.vertices[2].x(), face.vertices[2].y(), face.vertices[2].z());
          glVertex3f( face.vertices[3].x(), face.vertices[3].y(), face.vertices[3].z());

      }
       glEnd();

       glDisable(GL_BLEND);
       /*glDepthMask(GL_TRUE);*/


  }
}

void GLWidget::getAlpha(int alpha){

    _alphaNew = 1 - ((float)alpha/100.0);
    paintGL();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // TODO:
  // - Capture mouse motions
  // - If left click is pressed: rotate
  // - If right click is pressed: translate
    //get mouse position
    QPoint displace(0,0);
    QPoint mouse_pos = event->pos();
    /*Code to check in what direction is the button preesed*/
    if( event->buttons() == Qt::RightButton ||  event->buttons() == Qt::LeftButton)
    {
        if((mouse_pos.x()-old_point_t.x()) > 0)
        {
            //cout<<"right"<<endl;
            displace.setX(1);
            //translation.rx()+=0.01;
        }
        if((mouse_pos.x()-old_point_t.x()) < 0)
        {
            //cout<<"left"<<endl;
            displace.setX(-1);
            //translation.rx()-=0.01;
        }

        if((mouse_pos.y()-old_point_t.y()) > 0)
        {
            //cout<<"up"<<endl;
            displace.setY(-1);
            //translation.ry()-=0.01;
        }
        if((mouse_pos.y()-old_point_t.y()) < 0)
        {
            //cout<<"down"<<endl;
            displace.setY(1);
            //translation.ry()+=0.01;
        }
    }
     /***************************************************/

    /* Left button = rotation*/
    if( event->buttons() == Qt::LeftButton)
    {
        rotation.rx()=-displace.x();
        rotation.ry()=displace.y();

        rotation_angle = speed_factor*2;
    }
    /* Right button = Translation*/
    if( event->buttons() == Qt::RightButton)
    {
        translation.rx()= (pow(speed_factor,1.5)*0.02*displace.x());
        translation.ry()= (pow(speed_factor,1.5)*0.02*displace.y());
    }

    /*Update points*/
    old_point_t.setX(mouse_pos.x());
    old_point_t.setY(mouse_pos.y());

    QWidget::update();

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
  // TODO: zoom
    QPoint numDegrees = event->angleDelta();

    if (numDegrees.y() < 0)  zoomScale = (-1);
    if (numDegrees.y() > 0)  zoomScale = 1;
    zoomScale = pow((1.1*speed_factor),zoomScale);
    QWidget::update(); // call paintGL()
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() ==  Qt::Key::Key_Shift)
    {
        cout<<"SHIFT PRESSED"<<endl;
        speed_factor = 2;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() ==  Qt::Key::Key_Shift)
    {
        cout<<"SHIFT RELEASED"<<endl;
        speed_factor = 1;
    }
}
