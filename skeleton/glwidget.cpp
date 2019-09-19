
#include <QFile>
#include <QString>
#include <QTransform>
#include <QtGui>
#include <iostream>
#include "glwidget.h"

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent){}

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

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    setMouseTracking(true);
}

void GLWidget::paintGL() {
  QSize viewport_size = size();
  glViewport(0, 0, viewport_size.width(), viewport_size.height());


  glMatrixMode(GL_PROJECTION);
  // TODO: define the point of view
  glLoadIdentity();

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
  if(face_collection.init==true)
  {
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();                 // Reset the model-view matrix
      glTranslatef(translation.x(),translation.y(),0.0f);
      glRotatef(left_rot,1.0,0.0,1.0);
      /*glScalef(0.0, 0.0, 0.0);*/
      //***********//

      // TODO: draw the model

      // TODO: draw the model
      GLuint tex;
      glEnable(GL_TEXTURE_2D);
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      //glClearColor(1.0, 1.0, 1.0, 1.0);
      glDisable(GL_DEPTH_TEST);
      //glDepthMask(GL_FALSE);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      //      std::cout << _alphaNew << std::endl;
      for (int face_index=0;face_index<face_collection.faces.size();face_index++)
      {
          Face face = face_collection.faces[face_index];
          float color = face_index/(float)face_collection.faces.size();//face.c;//float)// //
          glColor4f(color, color, color, 1.0);     // Green

          glTexCoord2f(0.0f, 0.0f); glVertex3f( face.vertices[0].x(), face.vertices[0].y(), face.vertices[0].z());
          glTexCoord2f(1.0f, 0.0f); glVertex3f( face.vertices[1].x(), face.vertices[1].y(), face.vertices[1].z());
          glTexCoord2f(0.0f, 1.0f); glVertex3f( face.vertices[2].x(), face.vertices[2].y(), face.vertices[2].z());
          glTexCoord2f(1.0f, 1.0f); glVertex3f( face.vertices[3].x(), face.vertices[3].y(), face.vertices[3].z());
      }
      glEnd();  // End of drawing color-cube

      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
  }
}

float GLWidget::getAlpha(int alpha){

    _alphaNew = ((float)alpha/20.0);
    _angle = alpha*4;
    paintGL();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // TODO:
  // - Capture mouse motions
  // - If left click is pressed: rotate
  // - If right click is pressed: translate
    //get mouse position
    if( event->buttons() == Qt::RightButton )
    {
        cout<< "RightButton" <<endl;
        QPoint mouse_pos = event->pos();
        if((mouse_pos.x()-old_point_t.x()) > 0)
        {
            //cout<<"right"<<endl;
            translation.rx()+=0.01;
        }
        if((mouse_pos.x()-old_point_t.x()) < 0)
        {
            //cout<<"left"<<endl;
            translation.rx()-=0.01;
        }

        if((mouse_pos.y()-old_point_t.y()) > 0)
        {
            //cout<<"right"<<endl;
            translation.ry()-=0.01;
        }
        if((mouse_pos.y()-old_point_t.y()) < 0)
        {
            //cout<<"right"<<endl;
            translation.ry()+=0.01;
        }
        old_point_t.setX(mouse_pos.x());
        old_point_t.setY(mouse_pos.y());

        cout<< "LeftButton - Rotation" <<left_rot<< endl;
    }

    if(event->buttons() == Qt::LeftButton)
    {
        cout << "hi_right_buttonpress" << endl;

        QPoint mouse_pos = event->pos();
        if((mouse_pos.x()-old_point.x()) > 0)
        {
            cout<<"derecha"<<endl;
            left_rot+=1;
        }
        else
        {
            cout<<"izquierda"<<endl;
            left_rot-=1;
        }
        old_point.setX(mouse_pos.x());
        old_point.setY(mouse_pos.y());


        if(left_rot >= 360 )
        {
            left_rot =0;
        }
        cout<< "LeftButton - Rotation" <<left_rot<< endl;
    }
    //QWidget::event(event);
    QWidget::update();

}

void GLWidget::wheelEvent(QWheelEvent *event) {
  // TODO: zoom
}
