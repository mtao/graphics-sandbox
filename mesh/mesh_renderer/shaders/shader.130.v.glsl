#version 130
uniform mat4 MVP;
attribute vec3 vertex;
attribute float data;
varying float f_data;
varying float enable_phong;
void main(void)
{
    gl_Position = MVP * vec4(vertex,1);
    f_data = data;
    enable_phong = -1;
}
