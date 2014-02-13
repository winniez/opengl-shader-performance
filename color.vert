uniform vec3 co;
varying vec4 foo;

void main(void)
{
	//  Use color unchanged
   // gl_FrontColor = gl_Color;
   foo = gl_Vertex;//vec4(sin(co.x*100.0), cos(co.y*100.0), sin(co.z*100.0), 0.0);
   //  Set vertex coordinates
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//vec4 v = vec4(gl_Vertex);
	//v.z = sin(5.0*v.x )*0.25;

	//gl_Position = gl_ModelViewProjectionMatrix * v;
}