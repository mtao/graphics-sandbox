#version 130
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NT;
attribute vec3 vertex;
attribute float data;
varying float f_data;
attribute vec3 normal;
varying vec3 f_normal;
varying vec3 f_pos;
varying float enable_phong;
void main(void)
{
    gl_Position = MVP * vec4(vertex,1);
    f_data = 1;
    f_pos = (MV * vec4(vertex,1)).xyz;
    f_normal = (NT * vec4(normal,0)).xyz;
    enable_phong = 1;
}

