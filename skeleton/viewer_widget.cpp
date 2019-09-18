#include "viewer_widget.h"

#include <QFileDialog>

ViewerWidget::ViewerWidget() {
  layout = new QGridLayout(this);
  load_file_button = new QPushButton("Load file");
  layout->addWidget(load_file_button, 0, 0);
  connect(load_file_button, SIGNAL(released()), this, SLOT(loadFile()));
  gl_widget = new GLWidget();
  layout->addWidget(gl_widget, 1, 0);
}

void ViewerWidget::loadFile() {
  QString file_name;
  // TODO: open a dialog to let the user choose a json file
  gl_widget->loadFaces(file_name);
}
