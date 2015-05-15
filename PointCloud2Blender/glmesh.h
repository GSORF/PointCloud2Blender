#ifndef GLMESH_H
#define GLMESH_H

#include <QtOpenGL>
#include "importworker.h"

class Point3D;

class GLMesh
{
public:
    GLMesh(QOpenGLShaderProgram *shaderProgram, int vertexAttr, int colorAttr);
    ~GLMesh();

    void draw();

    void reset(bool meshed);
    void addPoint(Point3D &newPoint);
    void finished();

private:
    void initVertices();
    void initColors();

    int maxVertices;
    int currentVertex;
    bool meshed;
    bool lines;

    std::vector<float> vertices;
    std::vector<float> colors;

    QOpenGLShaderProgram *shaderProgram;
    int vertexAttribute;
    int colorAttribute;
};

#endif // GLMESH_H
