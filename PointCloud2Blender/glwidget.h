/*
    PointCloud2Blender - convert point cloud files to a textured 3D mesh
    Copyright (C) 2015 Adam Kalisz (Bachelor@Kalisz.co)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include "glmesh.h"

//Note: TODO: Fix Ubuntu issue with "QOpenGLWidget" not available! (on QT 5.4) ...

class GLMesh;

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    GLMesh *pointCloudMesh;
    void addPoint(Point3D newPoint, QVector3D translationVector);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void setCameraXRotation(float angle);
    void setCameraYRotation(float angle);
    void setCameraZRotation(float angle);

    float cameraXPosition;
    float cameraYPosition;
    float cameraZPosition;
    float cameraXRot;
    float cameraYRot;
    float cameraZRot;
    QPoint lastMousePosition;

    QOpenGLShaderProgram shaderProgram;

    int vertexAttributeID;
    int colorAttributeID;
    int matrixUniformID;

};

#endif // GLWIDGET_H
