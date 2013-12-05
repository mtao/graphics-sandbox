#version 330
uniform mat4 MVP;
in vec3 vertex;
void main(void)
{
    gl_Position = MVP * vec4(vertex,1);
}
