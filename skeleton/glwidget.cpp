#include <QFile>
#include <QString>
#include <QTransform>
#include <QtGui>

#include "glwidget.h"

#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

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

}
using namespace std;
int couter =0;
void GLWidget::paintGL() {
  QSize viewport_size = size();
  glViewport(0, 0, viewport_size.width(), viewport_size.height());
  couter++;


  glMatrixMode(GL_PROJECTION);
  // TODO: define the point of view

  GLdouble modelView[16];
  glGetDoublev (GL_PROJECTION_MATRIX, modelView);
  std::cout<<modelView[0]<<std::endl;
  if(couter <= 3)
  {
     glFrustum(-5,5,-5,5,2,7);
  }
  glLoadIdentity();


  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
  if(face_collection.init==true)
  {
      cout<<"Display the faces"<<endl;
      glMatrixMode(GL_MODELVIEW);
      //***********//

      // TODO: draw the model
      glLoadIdentity();                 // Reset the model-view matrix
      glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out

      for (int face_index=0;face_index<face_collection.faces.size();face_index++)
      {
          Face face = face_collection.faces[face_index];

          glColor3f(face.c, face.c, face.c);     // Green
          glVertex3f( face.vertices[1].x(), face.vertices[1].y(), face.vertices[1].y());
          glVertex3f( face.vertices[2].x(), face.vertices[2].y(), face.vertices[2].y());
          glVertex3f( face.vertices[3].x(), face.vertices[3].y(), face.vertices[3].y());
          glVertex3f( face.vertices[4].x(), face.vertices[4].y(), face.vertices[4].y());
      }
      glEnd();  // End of drawing color-cube
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // TODO:
  // - Capture mouse motions
  // - If left click is pressed: rotate
  // - If right click is pressed: translate
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  // TODO: zoom
}
