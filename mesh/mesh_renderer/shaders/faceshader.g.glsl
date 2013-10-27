#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
in float g_data[];
out float f_data;
in vec3 g_pos[];
out vec3 f_normal;
out vec3 f_pos;
void main()
{
    f_data = g_data[0];
    f_normal = normalize(cross(g_pos[1] - g_pos[0], g_pos[2] - g_pos[0]));
    gl_Position = gl_in[0].gl_Position;
    f_pos = g_pos[0];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    f_pos = g_pos[1];
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    f_pos = g_pos[2];
    EmitVertex();
    EndPrimitive();
}
