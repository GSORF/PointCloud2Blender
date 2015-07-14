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

#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget( parent ),
    cameraXPosition( 0.0f ),
    cameraYPosition( 0.0f ),
    cameraZPosition( 0.0f ),
    cameraXRot(0.0f),
    cameraYRot(0.0f),
    cameraZRot(0.0f)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete pointCloudMesh;
    doneCurrent();
}

void GLWidget::addPoint(Point3D newPoint, QVector3D translationVector)
{
    newPoint.x += translationVector.x();
    newPoint.y += translationVector.y();
    newPoint.z += translationVector.z();

    this->pointCloudMesh->addPoint(newPoint);
    this->update();
}



void GLWidget::initializeGL()
{
    glClearColor( .1f, .1f, .2f, 1.0f );

    //Bugfix: Wrong Z-Sorting:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    QOpenGLShader vertexShader( QOpenGLShader::Vertex );
    vertexShader.compileSourceFile(":/shaders/shaders/vertexshader.vert");

    QOpenGLShader fragmentShader( QOpenGLShader::Fragment );
    fragmentShader.compileSourceFile(":/shaders/shaders/fragmentshader.frag");

    shaderProgram.addShader( &vertexShader );
    shaderProgram.addShader( &fragmentShader );

    if(!shaderProgram.link())
    {
        qWarning() << shaderProgram.log();
        return;
    }

    vertexAttributeID = shaderProgram.attributeLocation("vertexAttribute");
    colorAttributeID = shaderProgram.attributeLocation("colorAttribute");
    matrixUniformID = shaderProgram.uniformLocation("matrix");

    pointCloudMesh = new GLMesh(&shaderProgram, vertexAttributeID, colorAttributeID);

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!shaderProgram.bind())
        return;

    //Orientation matrix:
    QMatrix4x4 matrix;
    matrix.perspective( 70.0f, 16.0f / 9.0f, 0.1f, 1400.0f);
    matrix.translate( this->cameraXPosition, this->cameraYPosition, this->cameraZPosition - 4.0f );
    matrix.rotate(this->cameraXRot, 1.0f, 0.0f, 0.0f );
    matrix.rotate(this->cameraYRot, 0.0f, 1.0f, 0.0f );
    matrix.rotate(this->cameraZRot, 0.0f, 0.0f, 1.0f );

    shaderProgram.setUniformValue( matrixUniformID, matrix );

    pointCloudMesh->draw();

    shaderProgram.release();

}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltax = event->x() - lastMousePosition.x();
    int deltay = event->y() - lastMousePosition.y();

    if(event->buttons() & Qt::RightButton)
    {
        setCameraXRotation(cameraXRot + deltay / 1.5f);
        setCameraZRotation(cameraZRot + deltax / 1.5f);
    }
    else if(event->buttons() & Qt::LeftButton)
    {
        setCameraXRotation(cameraXRot + deltay / 1.5f);
        setCameraYRotation(cameraYRot + deltax / 1.5f);
    }
    lastMousePosition = event->pos();


}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    cameraZPosition += event->delta() * .01f;
    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        cameraYPosition -= 0.5f;
        break;
    case Qt::Key_Down:
        cameraYPosition += 0.5f;
        break;
    case Qt::Key_Left:
        cameraXPosition += 0.5f;
        break;
    case Qt::Key_Right:
        cameraXPosition -= 0.5f;
        break;
    }

    update();

}


void GLWidget::setCameraXRotation(float angle)
{
    if(angle > 360)
    {
        angle -= 360;
    }
    else if(angle < 360)
    {
        angle += 360;
    }

    if(angle != cameraXRot)
    {
        cameraXRot = angle;
        update();
    }
}

void GLWidget::setCameraYRotation(float angle)
{
    if(angle > 360)
    {
        angle -= 360;
    }
    else if(angle < 360)
    {
        angle += 360;
    }

    if(angle != cameraYRot)
    {
        cameraYRot = angle;
        update();
    }
}

void GLWidget::setCameraZRotation(float angle)
{
    if(angle > 360)
    {
        angle -= 360;
    }
    else if(angle < 360)
    {
        angle += 360;
    }

    if(angle != cameraZRot)
    {
        cameraZRot = angle;
        update();
    }
}
