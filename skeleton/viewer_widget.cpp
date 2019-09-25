#include "viewer_widget.h"
#include <QResizeEvent>
#include <QFileDialog>

ViewerWidget::ViewerWidget() {
  layout = new QGridLayout(this);
  load_file_button = new QPushButton("Load file");
  layout->addWidget(load_file_button, 0, 0);
  connect(load_file_button, SIGNAL(released()), this, SLOT(loadFile()));
  gl_widget = new GLWidget();
  layout->addWidget(gl_widget, 1, 0);
  layout->addWidget(slider);
  slider-> setRange(0,100);
  slider-> setTickInterval(1);
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(alphaSlide(int)));
}

void ViewerWidget::loadFile() {  
  QString file_name= QFileDialog::getOpenFileName(this, ("Open File"),QDir::currentPath(), ("Document files (*.json)"));
  gl_widget->loadFaces(file_name);
}


void ViewerWidget::alphaSlide(int alpha){
    gl_widget-> getAlpha(alpha);
    gl_widget-> update();
}
void ViewerWidget::resizeEvent(QResizeEvent *event)
{
   static const float ratioY2X = 1;

   if (event->oldSize().width() != event->size().width())
      this->resize(event->size().width(),event->size().width() * ratioY2X);           // apply the correct ratio width/height
   else
      this->resize(event->size().height() / ratioY2X, event->size().height());   // apply the correct ratio

   event->accept(); // we've finished processing resize event here
}
