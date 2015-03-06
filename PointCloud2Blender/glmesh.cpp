#include "glmesh.h"

GLMesh::GLMesh(QOpenGLShaderProgram *shaderProgram, int vertexAttr, int colorAttr) :
    shaderProgram( shaderProgram ),
    vertexAttribute( vertexAttr ),
    colorAttribute( colorAttr )
{

    initVertices();
    initColors();
}

GLMesh::~GLMesh()
{

}

void GLMesh::draw()
{
    glPointSize(5);

    shaderProgram->setAttributeArray(vertexAttribute, vertices.data(), 3);
    shaderProgram->setAttributeArray(colorAttribute, colors.data(), 3);

    shaderProgram->enableAttributeArray( vertexAttribute );
    shaderProgram->enableAttributeArray( colorAttribute );

    glDrawArrays( GL_TRIANGLES, 0, vertices.size() / 3);

    shaderProgram->disableAttributeArray( vertexAttribute );
    shaderProgram->disableAttributeArray( colorAttribute );
}

void GLMesh::initVertices()
{
    vertices.resize(3*3);

    //First: Up
    vertices[0] = 0.0f;
    vertices[1] = 2.0f;
    vertices[2] = 0.0f;

    //Second: Right
    vertices[3] = 2.0f;
    vertices[4] = -2.0f;
    vertices[5] = 0.0f;

    //Third: Left
    vertices[6] = -2.0f;
    vertices[7] = -2.0f;
    vertices[8] = 0.0f;
}

void GLMesh::initColors()
{
    colors.resize(3*3);

    colors[0] = 0.0f;
    colors[1] = 0.0f;
    colors[2] = 1.0f;

    colors[3] = 0.0f;
    colors[4] = 1.0f;
    colors[5] = 0.0f;

    colors[6] = 1.0f;
    colors[7] = 0.0f;
    colors[8] = 0.0f;
}

