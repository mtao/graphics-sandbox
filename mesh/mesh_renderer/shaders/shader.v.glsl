#version 330
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NT;
in vec3 vertex;
in float data;
out float g_data;
out vec3 g_pos;
in vec3 normal;
out vec3 g_normal;
void main(void)
{
    gl_Position = MVP * vec4(vertex,1);
    g_pos = (MV * vec4(vertex,1)).xyz;
    g_data = data;
    g_normal = (NT*vec4(normal,0)).xyz;//normal;
}
