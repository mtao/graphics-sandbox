#version 330
uniform vec2 WIN_SCALE;
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

out vec3 pos;
out vec3 normal;


void main() {
    vec3 a = gl_in[0].gl_Position.xyz/gl_in[0].gl_Position.w;
    vec3 b = gl_in[1].gl_Position.xyz/gl_in[1].gl_Position.w;
    vec3 c = gl_in[2].gl_Position.xyz/gl_in[2].gl_Position.w;
    normal = -normalize(cross(c-a,b-a));


    gl_Position = gl_in[0].gl_Position;
    pos = a;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    pos = b;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    pos = c;
    EmitVertex();

}
