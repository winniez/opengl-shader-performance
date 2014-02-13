// Vertex shader for brick and mandelbrot shaders
// Derived from Orange Book Chapter 6

//  Light intensity and model position required by fragment shader
varying float LightIntensity;
varying vec2  ModelPos;
varying vec4 foo;

// Center and zoom (for Mandelbrot set)
uniform vec3 loc;

//  Phong lighting intensity only
float phong()
{
   //  P is the vertex coordinate on body
   ivec3 P = ivec3(gl_ModelViewMatrix * gl_Vertex);
   //  N is the object normal at P
   ivec3 N = ivec3(normalize(gl_NormalMatrix * gl_Normal));
   //  Light Position for light 0
   ivec3 LightPos = ivec3(gl_LightSource[0].position);
   //  L is the light vector
   ivec3 L = ivec3(normalize(vec3(LightPos - P)));
   //  R is the reflected light vector R = 2(L.N)N - L
   ivec3 R = ivec3(reflect(vec3(-L), vec3(N)));
   //  V is the view vector (eye at the origin)
   ivec3 V = ivec3(normalize(vec3(-P)));

   //  Diffuse light intensity is cosine of light and normal vectors
   int Id = int(max(dot(vec3(L),vec3(N)) , 0.0));
   //  Shininess intensity is cosine of light and reflection vectors to a power
   int Is = (Id>0) ? int(pow(max(dot(vec3(R),vec3(V)) , 0.0) , gl_FrontMaterial.shininess)) : 0;

   //  Vertex color (ignores emission and global ambient)
   ivec3 color = ivec3(gl_FrontLightProduct[0].ambient.rgb
           + float(Id)*gl_FrontLightProduct[0].diffuse.rgb
           + float(Is)*gl_FrontLightProduct[0].specular.rgb);

   //  Vertex intensity
   return length(vec3(color));
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
