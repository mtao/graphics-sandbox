#version 410
in vec2 UV;

layout(location=0) out vec4 color;
uniform sampler2D mysampler;

void main() {
    color = texture (mysampler, UV) ;
}
