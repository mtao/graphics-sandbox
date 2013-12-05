#version 130

varying float f_data;
varying vec3 f_pos;
varying vec3 f_normal;
varying float enable_phong;
void main(void)
{
    /*
    if(abs(f_data) < 0.004) {
        discard;
    }
    */
    if(enable_phong < 0) {
        gl_FragColor = vec4(f_data,0,-f_data,1);
        return;
    }
    vec3 eyepos = vec3(0,0,10);
    vec3 lightpos = vec3(0,10,10);
    vec3 eyedir = normalize(eyepos - f_pos);
    vec3 lightdir = normalize(eyepos - f_pos);
    float ambient = 0.1;
    float diffuse = max(dot(lightdir,f_normal),0);

    float lightdnormal = dot(f_normal,lightdir);
    float spec = float(lightdnormal>0)*max(.3*pow(max(dot(eyedir,2*lightdnormal*f_normal - lightdir),0),10),0);

    gl_FragColor = (ambient + diffuse + spec) * vec4(0.7,.7,.7,1) + vec4(f_data,0,-f_data,1);
}
