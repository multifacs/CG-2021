#ifndef SHADERWIDGET_H
#define SHADERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class ShaderWidget : public QOpenGLWidget {
private:
    QOpenGLShaderProgram m_program;
    GLfloat* vert_data;
    int m_position;//расположение массива в пределах списка параметров шейдерной программы
    QOpenGLFunctions_4_3_Core* functions;
    GLuint ssbo = 0;// shader storage buffer objects - тип буферных блоков, задаёт объект сцены, доступная функ-сть OpenGL c версии 4.3.
        //Чтобы передать массив сфер с хоста в шейдер необходимо и в шейдере, и в программе С++ создать структуру, описывающую сферу
    struct Sphere {
            QVector3D position;
            float radius;
            QVector3D color;
            int material_idx;
        };
protected:
    void initializeGL() override;
    void resizeGL(int nWidth, int nHeight) override;
    void paintGL() override;
public:
    ShaderWidget(QWidget *parent = 0);
    ~ShaderWidget();
};
#endif // SHADERWIDGET_H
