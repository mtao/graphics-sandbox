#version 330
layout(points) in;
layout(points, max_vertices=3) out;
in float g_data[];
out float f_data;

void main()
{
    f_data = g_data[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}
