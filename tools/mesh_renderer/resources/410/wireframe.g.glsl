#version 330
uniform vec2 WIN_SCALE;
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
noperspective out vec3 dist;

vec2 point(int i) {
    return WIN_SCALE * gl_in[i].gl_Position.xy/gl_in[i].gl_Position.w;
}

void main() {

    vec2 p0 = point(0);
    vec2 p1 = point(1);
    vec2 p2 = point(2);
    vec2 v0 = p2-p1;
    vec2 v1 = p1-p0;
    vec2 v2 = p2-p0;
    float area = abs(v1.x*v2.y-v1.y*v2.x);

    dist = vec3(area/length(v0),0,0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    dist = vec3(0,area/length(v1),0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    dist = vec3(0,0,area/length(v2));
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
