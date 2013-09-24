#version 130

varying float f_data;
varying vec3 pos;

void main() {
    if(!gl_FrontFacing)
        discard;

    gl_FragColor = vec4(2*f_data-1,1-abs(2*f_data-1),1-2*f_data,1);
//    gl_FragColor.xyz = pos;
}
