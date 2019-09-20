#pragma once

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QString>
#include <QWidget>
#include <QtGui>

#include "face.h"

#define SHIFT_KEY 0x1000020

class GLWidget : public QOpenGLWidget {
public:
  Q_OBJECT
    float _alphaNew = 1;
    float left_rot=0;
    float zoomScale=1;
    float speed_factor = 1;
    QPointF translation=QPoint(0.0f,0.0f);
    QPointF rotation=QPoint(0.0f,0.0f);

    float right_trans = 0;
    QPoint old_point=QPoint(0.0f,0.0f);
    QPoint old_point_t=QPoint(0.0f,0.0f);
public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();
  QSize sizeHint() const { return QSize(400, 400); }

  void loadFaces(const QString &path);
  float getAlpha(int);

protected:
  void initializeGL() override;
  void paintGL() override;

  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

  FaceCollection face_collection;
};
