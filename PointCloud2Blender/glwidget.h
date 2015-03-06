#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include "glmesh.h"

//Note: TODO: Fix Ubuntu issue with "QOpenGLWidget" not available! (on QT 5.4) ...

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void setCameraXRotation(float angle);
    void setCameraYRotation(float angle);
    void setCameraZRotation(float angle);

    float cameraDistance;
    float cameraXRot;
    float cameraYRot;
    float cameraZRot;
    QPoint lastMousePosition;

    GLMesh *pointCloudMesh;
    QOpenGLShaderProgram shaderProgram;

    int vertexAttributeID;
    int colorAttributeID;
    int matrixUniformID;

};

#endif // GLWIDGET_H
