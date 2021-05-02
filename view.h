#pragma once

#include "data.h"

#include <QtOpenGL>
#include <QtWidgets/QOpenGLWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/qtopenglglobal.h>

class View : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
protected:
    Data data_;
    int layer_;
    enum {kVisualisationQuads, kVisualisationQuadStrip, kVisualisationTexture} visualisation_state_;
    enum {x, y, z} axis_cut_;

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

    int getState() { return visualisation_state_; }

    View(QWidget *parent = nullptr);
    void LoadData(std::string filename);

public slots:
    void SetMin(short value);
    void SetMax(short value);

    void PressW();
    void PressS();
    void PressN();

    void SetX() { axis_cut_ = x; update(); }
    void SetY() { axis_cut_ = y; update(); }
    void SetZ() { axis_cut_ = z; update(); }

signals:
    void GetMin(short value);
    void GetMax(short value);


protected slots:

};
