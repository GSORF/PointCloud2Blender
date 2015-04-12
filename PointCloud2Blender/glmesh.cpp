#include "glmesh.h"

GLMesh::GLMesh(QOpenGLShaderProgram *shaderProgram, int vertexAttr, int colorAttr) :
    shaderProgram( shaderProgram ),
    vertexAttribute( vertexAttr ),
    colorAttribute( colorAttr )
{

    maxVertices = 3000000;
    currentVertex = 0;
    meshed = false;

    initVertices();
    initColors();
}

GLMesh::~GLMesh()
{

}

void GLMesh::draw()
{
    glPointSize(5.0);

    shaderProgram->setAttributeArray(vertexAttribute, vertices.data(), 3);
    shaderProgram->setAttributeArray(colorAttribute, colors.data(), 3);

    shaderProgram->enableAttributeArray( vertexAttribute );
    shaderProgram->enableAttributeArray( colorAttribute );

    if(meshed)
    {
        glDrawArrays( GL_QUADS, 0, vertices.size() / 3);
    }
    else
    {
        glDrawArrays( GL_POINTS, 0, vertices.size() / 3);
    }

    shaderProgram->disableAttributeArray( vertexAttribute );
    shaderProgram->disableAttributeArray( colorAttribute );
}

void GLMesh::reset(bool meshed)
{
    this->meshed = meshed;

    for(unsigned int i = 0; i < (vertices.size() / 3); i+=3)
    {
        vertices[i*3 + 0] = 0.0f;
        vertices[i*3 + 1] = 0.0f;
        vertices[i*3 + 2] = 0.0f;
    }
    for(unsigned int i = 0; i < (colors.size() / 3); i+=3)
    {
        colors[i*3 + 0] = 0.0f;
        colors[i*3 + 1] = 0.0f;
        colors[i*3 + 2] = 0.0f;
    }

    currentVertex = 0;
}

void GLMesh::addPoint(Point3D &newPoint)
{
    if(currentVertex < maxVertices)
    {
        vertices[currentVertex*3 + 0] = newPoint.x;
        vertices[currentVertex*3 + 1] = newPoint.y;
        vertices[currentVertex*3 + 2] = newPoint.z;

        colors[currentVertex*3 + 0] = newPoint.r / 255.0f;
        colors[currentVertex*3 + 1] = newPoint.g / 255.0f;
        colors[currentVertex*3 + 2] = newPoint.b / 255.0f;

        currentVertex++;
    }
    else
    {
        reset(meshed);
    }
}

void GLMesh::finished()
{
    //gets called when a mesh has been filled with vertices
    //initiate a draw command or make all vertices visible

    //for now, just reset
    reset(true);
}

void GLMesh::initVertices()
{
    vertices.resize(3000000*3);

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
    colors.resize(3000000*3);

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

