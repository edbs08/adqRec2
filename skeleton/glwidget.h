#pragma once

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QString>

#include "face.h"

class GLWidget : public QOpenGLWidget {
public:
  Q_OBJECT
    float _alphaNew = 1;
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

  FaceCollection face_collection;
};
