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
