#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLFunctions_3_3_Compatibility>
#include "mainwindow.h"
#include "../3rdparty/glm/glm.hpp"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Compatibility {
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr, QString nm = "") : QOpenGLWidget(parent) { file_nm = nm; }
    ~OpenGLWidget() {}

    void RenderObjFile();

private:
    void CirclePaint(GLuint shaderProgram, const std::vector<glm::vec3>& vertices, GLuint VAO); // correspond to while circle in VS opengl
    bool LoadObj(std::vector<glm::vec3>& out_vertices);


protected:

private:
    QString file_nm;
};


#endif // OPENGLWIDGET_H
