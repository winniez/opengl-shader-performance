/*
 *  Shader Performance Homework
 *
 *  'p' to switch projections
 *  'o' to increase number of objects
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 */
#include "CSCIx239.h"
#include "Object.h"
#include "Cube.h"
#include "Sphere.h"
#include "WaveOBJ.h"
#include "object.c"
#include<vector>
#define MODE 8

using namespace std;

int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int movelight = 1;       //  Move light
int roll=1;       //  Rolling brick texture
int proj=0;       //  Projection type
int obj=0;        //  Object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=10.0;   //  Size of world
int zh=90;        //  Light azimuth
float Ylight=2;   //  Light elevation
vector<Object*> objs;  //  Array of objects
vector<int> shader(MODE, 0); //  Shader program
char* text[] = {"No Shader", 
                "Brick", "int Brick", "self defined func brick",
                "Mandelbrot", "int Mandelbrot", "self defined func mandelbrot",
                "Color"};
// I use int instead of float in int Brick and int Mandelbrot shader.
// I explicit define mix, pow, max, normalize, reflect, fract function in self defined brick and self defind func mandelbrot

float X=0,Y=0,Z=1; //  Mandelbrot X,Y,Z

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    const double len=2.0;  //  Length of axes
    //  Light position and colors
    float Emission[]  = {0.0,0.0,0.0,1.0};
    float Ambient[]   = {0.3,0.3,0.3,1.0};
    float Diffuse[]   = {1.0,1.0,1.0,1.0};
    float Specular[]  = {1.0,1.0,1.0,1.0};
    float Position[]  = {2*Cos(zh),Ylight,2*Sin(zh),1.0};
    float Shinyness[] = {16};
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);

    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    if (proj)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
    //  Draw light position as sphere (still no lighting here)
   glColor3f(1,1,1);
   glPushMatrix();
   glTranslated(Position[0],Position[1],Position[2]);
   glutSolidSphere(0.03,10,10);
   glPopMatrix();
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   //  Set materials
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

    //  Select shader (0 => no shader)
    glUseProgram(shader[mode]);

   //  Export time to uniform variable
    float time = roll ? 0.001*glutGet(GLUT_ELAPSED_TIME) : 0;
    if (mode)
    {
        int id;
        id = glGetUniformLocation(shader[mode],"loc");
        if (id>=0) glUniform3f(id,X,Y,Z);
        id = glGetUniformLocation(shader[mode],"time");
        if (id>=0) glUniform1f(id,time);
    }

    //  Draw the model, teapot or cube
    glColor3f(1,1,0);
    
    for (int k = 0; k < obj+1; k++)
    {
        objs[k]->display();
    }
    for (int k = 0; k < obj; k++)
    {
        ((WaveOBJ*)objs[k])->rotate((float)k,1.0,0.0,0.0);
    }

    //  No shader for what follows
    glUseProgram(0);

    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s %s fps=%d",
     th,ph,dim,proj?"Perpective":"Orthogonal",text[mode], FramesPerSecond());
    if (mode>1)
   {
      glWindowPos2i(5,25);
      Print("X=%f Y=%f Z=%f Mag=%f",X,Y,Z,1/Z);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int xch,int ych)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int xch,int ych)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
    //  Toggle axes
    else if (ch == 'a' || ch == 'A')
        axes = 1-axes;
    //  Toggle projection type
    else if (ch == 'p' || ch == 'P')
        proj = 1-proj;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      movelight = 1-movelight;
   //  Toggle brick movement
   else if (ch == 'b' || ch == 'B')
      roll = 1-roll;
    //  Toggle objects
    else if (ch == 'o' || ch == 'O')
        obj = (obj+1)%objs.size();
    //  Cycle modes
    else if (ch == 'm')
        mode = (mode+1)%MODE;
    else if (ch == 'M')
        mode = (mode+MODE-1)%MODE;
   //  Light elevation
   else if (ch == '+')
      Ylight += 0.1;
   else if (ch == '-')
      Ylight -= 0.1;
   //  Light position
   else if (ch == '[')
      zh--;
   else if (ch == ']')
      zh++;
   //  Mandelbrot
   else if (ch == 'x')
      X += 0.01*Z;
   else if (ch == 'X')
      X -= 0.01*Z;
   else if (ch == 'y')
      Y += 0.01*Z;
   else if (ch == 'Y')
      Y -= 0.01*Z;
   else if (ch == 'z')
      Z *= 0.9;
   else if (ch == 'Z')
      Z *= 1.1;
    else if (ch == 'd')
        dim += 0.1;
    else if (ch == 'D')
        dim -= 0.1;
   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}


/*
 *  GLUT calls this routine every 50ms
 */
void timer(int k)
{
   glutPostRedisplay();
   glutTimerFunc(50,timer,0);
}

/*
 *  Initialize object states
 */
void Init()
{
    objs.push_back(new Sphere(-1.5,-7.5, -1.5, 0.5, 1.0,0.0,0.0));
    objs.push_back(new WaveOBJ("bunny.obj"));
    ((WaveOBJ*)objs[1])->translate(-1.0, -6.5, -1.0);
    ((WaveOBJ*)objs[1])->scale(4.0);
    //((WaveOBJ*)objs[1])->rotate(5.0,1.0,1.0,1.0);
    objs.push_back(new WaveOBJ("armadillo.obj"));
    ((WaveOBJ*)objs[2])->translate(1.0, -5.5, -1.5);
    ((WaveOBJ*)objs[2])->scale(0.6);
    //((WaveOBJ*)objs[2])->rotate(8.0,1.0,1.0,1.0);
    objs.push_back(new WaveOBJ("tyra.obj"));
    ((WaveOBJ*)objs[3])->translate(0.0, -4.0, 0.0);
    ((WaveOBJ*)objs[3])->scale(2.0);
    //((WaveOBJ*)objs[3])->rotate(1.0,1.0,1.0,1.0);
    objs.push_back(new WaveOBJ("armadillo.obj"));
    ((WaveOBJ*)objs[4])->translate(1.5, -3.5, 1.5);
    ((WaveOBJ*)objs[4])->scale(0.5);
    //((WaveOBJ*)objs[4])->rotate(3.0,1.0,0.0,1.0);
    objs.push_back(new WaveOBJ("mba2.obj"));
    ((WaveOBJ*)objs[5])->translate(0.0, -3.0, 0.0);
    ((WaveOBJ*)objs[5])->scale(0.07);
    //((WaveOBJ*)objs[5])->rotate(5.0,1.0,0.0,0.0);
}

/*
 *  Set object states
 */
void Set(float zh)
{ 
   ((WaveOBJ*)objs[1])->rotate(zh,1,0,0);
   ((WaveOBJ*)objs[2])->rotate(zh,0,1,0);
   ((WaveOBJ*)objs[3])->rotate(zh,0,0,1);
   ((WaveOBJ*)objs[4])->rotate(zh,1,0,0);
   //((WaveOBJ*)objs[5])->rotate(zh,0,1,0);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    //  Translate time into 90 degree/sec angle
    if (movelight) zh = fmod(90*t,360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    Set(fmod(90*t, 360.0));
    glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Procedural Texture");
#ifdef USEGLEW
    //  Initialize GLEW
    if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
    if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    //  Load texture
    LoadTexBMP("pi.bmp");
    //  Create Shader Programs
    //shader[1] = CreateShaderProg("stored.vert","stored.frag");
    shader[1] = CreateShaderProg("model.vert","brick.frag");
    shader[2] = CreateShaderProg("int_model.vert", "brick.frag");
    shader[3] = CreateShaderProg("self_func_model.vert", "self_func_brick.frag");
    shader[4] = CreateShaderProg("model.vert","mandel.frag");
    shader[5] = CreateShaderProg("int_model.vert", "int_mandel.frag");
    shader[6] = CreateShaderProg("self_func_model.vert", "self_func_mandel.frag");
    shader[7] = CreateShaderProg("model.vert", "color.frag");
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    Init();
    glutMainLoop();
    return 0;
}


