#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
uniform mat4 MVP;
uniform mat4 MV;
varying vec3 normal;
varying vec3 pos;
void main() {
    vec4 v4pos = vec4(vertexPosition,1.0);
    gl_Position = MVP * v4pos;
    gl_Position = v4pos;
    v4pos = MV * v4pos;
    pos = v4pos.xyz;

    normal = vertexNormal;
}
