#version 410

layout(location=0) out vec4 fragColor;
uniform vec4 color;

noperspective in vec3 dist;

void main() {
    float d = min(dist.x,min(dist.y,dist.z));
    float i = exp2(-2*d*d);
    if(i<0.2) {
        fragColor = vec4(0,0,0,0);
        fragColor.xyz =  color.xyz * 0.2;
        fragColor.w = 0.01;
    } else {
        fragColor = i * color;
    }
}
