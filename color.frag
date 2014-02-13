
varying vec4 foo;
varying float LightIntensity;
void main()
{
    gl_FragColor = foo*LightIntensity;
} 