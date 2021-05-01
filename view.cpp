#include "view.h"

View::View(QWidget *parent) : QOpenGLWidget(parent), data_(), layer_(0), visualisation_state_(kVisualisationQuads) {}

void View::LoadData(std::string filename) {
    data_.ReadFile(filename);
}

void View::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void View::resizeGL(int n_width, int n_height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, data_.GetWidth() - 1, 0.f, data_.GetHeight() - 1, -1.f, 1.f);
    glViewport(0, 0, n_width, n_height);
    update();
}

void View::paintGL() {
    qDebug() << "repaint" << visualisation_state_;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (visualisation_state_) {
        case kVisualisationQuads:
            VisualisationQuads();
            break;
        case kVisualisationQuadStrip:
            //VisualisationQuadStrip();
            break;
        case kVisualisationTexture:
            //VisualisationTexture();
            break;
    }
}

void View::VisualisationQuads() {
    float c;
    int w = data_.GetWidth();
    int h = data_.GetHeight();

    for (int y = 0; y < h - 1; y++) {
        for (int x = 0; x < w - 1; x++) {
            glBegin(GL_QUADS);

            c = TransferFunction(data_[layer_ * w * h + y * w + x]);
            glColor3f(c, c, c);
            glVertex2i(x, y);

            c = TransferFunction(data_[layer_ * w * h + (y + 1) * w + x]);
            glColor3f(c, c, c);
            glVertex2i(x, y + 1);

            c = TransferFunction(data_[layer_ * w * h + (y + 1) * w + x + 1]);
            glColor3f(c, c, c);
            glVertex2i(x + 1, y + 1);

            c = TransferFunction(data_[layer_ * w * h + y * w + x + 1]);
            glColor3f(c, c, c);
            glVertex2i(x + 1, y);

            glEnd();
        }
    }
}

float View::TransferFunction(short value) {
    return float(value - data_.GetMin()) / float(data_.GetMax() - data_.GetMin());
}

void View::keyPressEvent(QKeyEvent *event) {
    quint32 key_pressed = event->nativeVirtualKey();
    if (key_pressed == Qt::Key_U || key_pressed == Qt::Key_Up) {
        layer_ = std::min(layer_ + 1, data_.GetDepth() - 1);
        //layer_ = ((layer_ + 2) < data_.GetDepth()) * (layer_ + 1);
    } else if (key_pressed == Qt::Key_D || key_pressed == Qt::Key_Down) {
        layer_ = std::max(layer_ - 1, 0);
        //layer_ = (layer_ > 0) * (layer_ - 1);
    } else if (key_pressed == Qt::Key_N) {
        switch (visualisation_state_) {
            case kVisualisationQuads:
                visualisation_state_ = kVisualisationQuadStrip; break;
            case kVisualisationQuadStrip:
                visualisation_state_ = kVisualisationTexture; break;
            case kVisualisationTexture:
                visualisation_state_ = kVisualisationQuads; break;
        }
    }

    update();
}
