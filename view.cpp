#include "view.h"

View::View(QWidget *parent) : QOpenGLWidget(parent), data_(), layer_(0), visualisation_state_(kVisualisationQuads), axis_cut_(x) {}

void View::LoadData(std::string filename)
{
    data_.ReadFile(filename);
}

void View::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glGenTextures(1, &VBOtexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void View::resizeGL(int n_width, int n_height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, data_.GetWidth() - 1, 0.f, data_.GetHeight() - 1, -1.f, 1.f);
    glViewport(0, 0, n_width, n_height);
    update();
}

void View::paintGL()
{
    qDebug() << "repaint" << visualisation_state_;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (visualisation_state_)
    {
        case kVisualisationQuads:
            VisualisationQuads();
            break;
        case kVisualisationQuadStrip:
            VisualisationQuadStrip();
            break;
        case kVisualisationTexture:
            VisualisationTexture();
            break;
    }
    qDebug() << "MIN: " << data_.GetMin();
    qDebug() << "MAX: " << data_.GetMax();
}

void View::VisualisationQuads()
{
    float c;
    int w = data_.GetWidth();
    int h = data_.GetHeight();
    int d = data_.GetDepth();

    switch (axis_cut_)
    {
        case x:
        for (int y = 0; y < h - 1; y++)
        {
            for (int x = 0; x < w - 1; x++)
            {
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
        break;

    case y:
        for (int y = 0; y < h - 1; y++)
        {
            for (int z = 0; z < d - 1; z++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data_[w * h * z + y * w + layer_]);
                glColor3f(c, c, c);
                glVertex2i(z, y);

                c = TransferFunction(data_[w * h * z + (y + 1) * w + layer_]);
                glColor3f(c, c, c);
                glVertex2i(z, y + 1);

                c = TransferFunction(data_[w * h * (z + 1) + (y + 1) * w + layer_]);
                glColor3f(c, c, c);
                glVertex2i(z + 1, y + 1);

                c = TransferFunction(data_[w * h * (z + 1) + y * w + layer_]);
                glColor3f(c, c, c);
                glVertex2i(z + 1, y);
                glEnd();
            }
        }
        break;

    case z:
        for (int z = 0; z < d - 1; z++)
        {
            for (int x = 0; x < w - 1; x++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data_[x + z * w * h + layer_ * h]);
                glColor3f(c, c, c);
                glVertex2i(x, z);

                c = TransferFunction(data_[x + (z + 1) * w * h + layer_ * h]);
                glColor3f(c, c, c);
                glVertex2i(x, z + 1);

                c = TransferFunction(data_[(x + 1) + (z + 1) * w * h + layer_ * h]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z + 1);

                c = TransferFunction(data_[(x + 1) + z * w * h + layer_ * h]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z);
                glEnd();
            }
        }
        break;
    }
}

void View::VisualisationQuadStrip()
{
    float c;
    int w = data_.GetWidth();
    int h = data_.GetHeight();

    for (int y = 0; y < h - 1; y++)
    {
            glBegin(GL_QUAD_STRIP);
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data_[layer_ * w * h + y * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, y);

                c = TransferFunction(data_[layer_ * w * h + (y + 1) * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, y + 1);
            }
            glEnd();
        }
}

void View::VisualisationTexture()
{
    genTextureImage();
    Load2dTexture();

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(0, 0);
    glTexCoord2f(0, 1);
    glVertex2i(0, data_.GetHeight());
    glTexCoord2f(1, 1);
    glVertex2i(data_.GetWidth(), data_.GetHeight());
    glTexCoord2f(1, 0);
    glVertex2i(data_.GetWidth(), 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

float View::TransferFunction(short value)
{
    return float(value - data_.GetMin()) / float(data_.GetMax() - data_.GetMin());
}

void View::SetMin(short value)
{
    qDebug() << "SET_MIN " << value;
    data_.SetMin(value);
    update();
}

void View::SetMax(short value)
{
    qDebug() << "SET_MAX " << value;
    data_.SetMax(value);
    update();
}


void View::Load2dTexture()
{
    qDebug() << "LOAD_TEXTURE";
    glBindTexture(GL_TEXTURE_2D, VBOtexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
    0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void View::genTextureImage()
{
    int w = data_.GetWidth();
    int h = data_.GetHeight();
    textureImage = QImage(w, h, QImage::Format_RGB32);
    qDebug() << "GEN_TEXTURE";
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
        float c = TransferFunction(data_[layer_ * w * h + w * y + x]) * 255;
        if (c > 255)
            c = 255;
        else if (c < 0)
            c = 0;
        textureImage.setPixelColor(x, y, QColor(c,c,c));
    }
}

void View::PressW()
{
    layer_ = std::min(layer_ + 1, data_.GetDepth() - 1);
    update();
}

void View::PressS()
{
    layer_ = std::max(layer_ - 1, 0);
    update();
}

void View::PressN()
{
    switch (visualisation_state_)
    {
        case kVisualisationQuads:
            visualisation_state_ = kVisualisationQuadStrip; break;
        case kVisualisationQuadStrip:
            visualisation_state_ = kVisualisationTexture; break;
        case kVisualisationTexture:
            visualisation_state_ = kVisualisationQuads; break;
    }
    update();
}

