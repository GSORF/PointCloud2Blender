attribute vec4 vertexAttribute;
attribute vec4 colorAttribute;
uniform mat4 matrix;
varying vec4 color;

void main(void)
{
    gl_Position = matrix * vertexAttribute;
    color = colorAttribute;
}

