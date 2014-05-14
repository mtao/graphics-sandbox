#version 120

varying vec3 pos;
varying vec3 normal;
uniform vec4 color;
uniform vec3 eyePosition;
uniform vec3 lightPosition;


const vec3 diffuseMaterial = vec3(0.7, 0.7, 0.7);
const vec3 specularMaterial = vec3(0.99, 0.99, 0.99);
const vec3 ambientMaterial = vec3(0.1, 0.1, 0.1);

void main( void )
{
    // Calculate intensity as max of 0 and dot product of the
    // fragNormal and the eye position (0,0,1).
    float intensity;

    vec3 eyeDir = normalize(eyePosition-pos);//world
    vec3 lightDir=normalize(lightPosition - pos);


    vec3 ambient = ambientMaterial*color.xyz;

    vec3 worldNormal = normal;
    if(!gl_FrontFacing)
    {
        worldNormal = -worldNormal;
    }
    // calculate diffuse component
    float lightAng = max(0.0,dot(worldNormal, lightDir));
    if(lightAng == 0.0)
    {
        gl_FragColor = vec4(ambient,1.0);
        return;
    }
    vec3 diffuse = lightAng * color.rgb;

    // calculate specular component
    vec3 reflection = normalize(reflect(lightDir, worldNormal));
    float spec = max(0.0,dot(normalize(eyeDir), reflection));
    vec3 specular = pow(spec, 32.0) * specularMaterial;



    gl_FragColor = vec4(ambient+diffuse+specular,color.a);

}
