#pragma once

#include "data.h"

#include <QtOpenGL>
#include <QtWidgets/QOpenGLWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/qtopenglglobal.h>
//#include <QtOpenGL/qopenglfunctions_2_0.h>

class View : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
protected:
    Data data_;
    int layer_;
    enum {kVisualisationQuads, kVisualisationQuadStrip, kVisualisationTexture} visualisation_state_;

    void initializeGL() override;
    void resizeGL(int n_width, int n_height) override;
    void paintGL() override;

    void VisualisationQuads();
    void VisualisationQuadStrip();
    void VisualisationTexture();

    float TransferFunction(short value);

    GLuint VBOtexture;
    void Load2dTexture();
    QImage textureImage;
    void genTextureImage();
public:
    View(QWidget *parent = nullptr);
    void LoadData(std::string filename);

protected slots:
    void keyPressEvent(QKeyEvent *event) override;
};
