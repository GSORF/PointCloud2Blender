#ifndef GLMESH_H
#define GLMESH_H

#include <QtOpenGL>

class GLMesh
{
public:
    GLMesh(QOpenGLShaderProgram *shaderProgram, int vertexAttr, int colorAttr);
    ~GLMesh();

    void draw();

private:
    void initVertices();
    void initColors();

    std::vector<float> vertices;
    std::vector<float> colors;

    QOpenGLShaderProgram *shaderProgram;
    int vertexAttribute;
    int colorAttribute;
};

#endif // GLMESH_H
