// Vertex shader for brick and mandelbrot shaders
// Derived from Orange Book Chapter 6

//  Light intensity and model position required by fragment shader
varying float LightIntensity;
varying vec2  ModelPos;
varying vec4 foo;

// Center and zoom (for Mandelbrot set)
uniform vec3 loc;

// mix
vec3 mix_func(vec3 v1, vec3 v2, float a){
 return vec3((1.0-a)*v1.x+a*v2.x,(1.0-a)*v1.y+a*v2.y,(1.0-a)*v1.z+a*v2.z);

}

// dot
float dot_func(vec3 v1, vec3 v2){
 return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

// normalize
vec3 normalize_func(vec3 v){
  float deno = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
 return vec3(v.x/deno,v.y/deno,v.z/deno);
}

// max
float max_func(float a, float b)
{
   return (a > b) ? a : b;
}

// pow
float pow_func(float base, float exponent)
{
   if (exponent == 0.0) return 1.0;
   else return exp( exp(log(exponent) + log(log(base))));
}

// reflect
vec3 reflect_func(vec3 v1, vec3 v2){
  float d = dot_func(v1,v2);
  vec3 n = 2.0*v2*d;
  return vec3(v1.x-n.x,v1.y-n.y,v1.z-n.z);
}


//  Phong lighting intensity only
float phong()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  N is the object normal at P
   vec3 N = normalize_func(gl_NormalMatrix * gl_Normal);
   //  Light Position for light 0
   vec3 LightPos = vec3(gl_LightSource[0].position);
   //  L is the light vector
   vec3 L = normalize_func(LightPos - P);
   //  R is the reflected light vector R = 2(L.N)N - L
   vec3 R = reflect_func(-L, N);
   //  V is the view vector (eye at the origin)
   vec3 V = normalize_func(-P);

   //  Diffuse light intensity is cosine of light and normal vectors
   float Id = max(dot_func(L,N) , 0.0);
   //  Shininess intensity is cosine of light and reflection vectors to a power
   float Is = (Id>0.0) ? pow_func(max_func(dot_func(R,V) , 0.0) , gl_FrontMaterial.shininess) : 0.0;

   //  Vertex color (ignores emission and global ambient)
   vec3 color = gl_FrontLightProduct[0].ambient.rgb
           + Id*gl_FrontLightProduct[0].diffuse.rgb
           + Is*gl_FrontLightProduct[0].specular.rgb;

   //  Vertex intensity
   return length(color);
}

void main()
{
   foo = gl_Vertex;
   //  Scalar light intensity (for fragment shader)
   LightIntensity = phong();
   //  Save model coordinates (for fragment shader)
   ModelPos = loc.z*gl_Vertex.xy - loc.xy;
   //  Return fixed transform coordinates for this vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
