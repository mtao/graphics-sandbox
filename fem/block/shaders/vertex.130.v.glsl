#version 130

attribute vec4 vecl;
varying float f_data;
varying vec3 pos;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
void main() {

//    gl_Position = gl_Vertex;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vecl.xyz,1);

//    gl_Position = viewMatrix * vec4(vecl.xyz,1);
    pos = vecl.xyz;

    f_data = vecl.w;

}
