#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
in vec3 g_pos[];
in vec3 g_normal[];
out vec3 f_normal;
out vec3 f_pos;
out float f_data;
uniform mat4 P;
uniform float normal_offset;
void main()
{
    f_data = 0;
    f_normal = normalize(cross(g_pos[1] - g_pos[0], g_pos[2] - g_pos[0]));
    f_normal = g_normal[0];
    gl_Position = gl_in[0].gl_Position;
    gl_Position = P * vec4(g_pos[0] - f_normal * normal_offset,1);
    f_pos = g_pos[0];
    EmitVertex();
    f_normal = g_normal[1];
    gl_Position = gl_in[1].gl_Position;
    gl_Position = P * vec4(g_pos[1] - f_normal * normal_offset,1);
    f_pos = g_pos[1];
    EmitVertex();
    f_normal = g_normal[2];
    gl_Position = gl_in[2].gl_Position;
    gl_Position = P * vec4(g_pos[2] - f_normal * normal_offset,1);
    f_pos = g_pos[2];
    EmitVertex();
    EndPrimitive();
}
