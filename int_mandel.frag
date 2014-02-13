// Fragment shader for drawing the Mandelbrot set
// Adapted from Orange book

varying vec2  ModelPos;
varying float LightIntensity;

// Maximum number of iterations
const int   MaxIter = 100;
// Beyond this the sequence always diverges
const int MaxR2   = 4;

// Colors
const ivec3  In   = ivec3(0,0,0);
const ivec3  Out1 = ivec3(1,0,0);
const ivec3  Out2 = ivec3(0,1,0);

void main()
{
   //  Iterate z = z^2 + c;
   int   iter;
   ivec2  z  = ivec2(0,0);
   int r2 = 0;
   for (iter=0 ; iter<MaxIter && r2<MaxR2 ; iter++)
   {
       z  = ivec2(z.x*z.x-z.y*z.y , 2*z.x*z.y) + ivec2(ModelPos);
       r2 = z.x*z.x+z.y*z.y;
   }

   // Base the color on the number of iterations
   ivec3 color = (r2<MaxR2) ? In : ivec3(mix(vec3(Out1),vec3(Out2),float(iter)/float(MaxIter)));
   gl_FragColor = vec4(color*int(LightIntensity) , 1);
}
