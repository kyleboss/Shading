
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>


#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport  viewport;
/*
typedef struct {
  float r;
  float g;
  float b;
  void Set(float R, float G, float B){r=R, g=G, b=B;}
  float dotProduct(Color color2){return r*color2.r + g*color2.g + b*color2.b;}
  Color normify() {
    Color newColor;
    float norm = sqrt(r*r + g*g + b*b);
    float newR = r / norm;
    float newG = g / norm;
    float newB = b / norm;
    newColor.Set(newR, newG, newB);
    return newColor;
  }
} Color;

typedef struct {
  float x;
  float y;
  float z;
  void Set(float X, float Y, float Z){x=X, y=Y, z=Z;}
  float dotProduct(Pos pos2){return x*pos2.x + y*pos2.y + z*pos2.z;}
  Pos normify() {
    Pos newPos;
    float norm = sqrt(x*x + y*y + z*z);
    float newX = x / norm;
    float newY = y / norm;
    float newZ = z / norm;
    newPos.Set(newX, newY, newZ);
    return newPos;
  }
} Pos;

typedef struct {
  Color color;
  Pos pos;
  void Set(Color setColor, Pos setPos){color=setColor, pos=setPos;}
} Light;

Color ka;
Color kd;
Color ks;
int numDl = 0;
int numPl = 0;
Light dl[] = {Light dl0, Light dl1, Light dl2, Light dl3, Light dl4};
Light pl[] = {Light pl0, Light pl1, Light pl2, Light pl3, Light pl4};*/

int numDl = 0;
int numPl = 0;
int totalLights = 0;
int dl[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3};
int pl[] = {GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

float getLength(float v[]) {
  float x = v[0];
  float y = v[1];
  float z = v[2];
  return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

// float *getNorm(float v[]) {
//   float x = v[0];
//   float y = v[1];
//   float z = v[2];
//   float length = getLength(v);
//   float normX = x / length;
//   float normY = y / length;
//   float normZ = z / length;
//   float toRet[] = {normX, normY, normZ};
//   return toRet;
// }

//****************************************************
// Simple init function
//****************************************************
void initScene(int argc, char *argv[]){
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_RESCALE_NORMAL);
  for (int i = 1; i < argc; ++i) {
    if(strcmp(argv[i], "-ka") == 0) {
      float r = strtof(argv[i+1], NULL);
      float g = strtof(argv[i+2], NULL);
      float b = strtof(argv[i+3], NULL);
      GLfloat ambient[] = {r,g,b,1};
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
      i += 3;
    }
    if(strcmp(argv[i], "-kd") == 0) {
      float r = strtof(argv[i+1], NULL);
      float g = strtof(argv[i+2], NULL);
      float b = strtof(argv[i+3], NULL);
      GLfloat diffuse[] = {r,g,b,1};
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
      i += 3;
    } 
    if(strcmp(argv[i], "-ks") == 0) {
      float r = strtof(argv[i+1], NULL);
      float g = strtof(argv[i+2], NULL);
      float b = strtof(argv[i+3], NULL);
      GLfloat specular[] = {r,g,b,1};
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
      i += 3;
    }
    if(strcmp(argv[i], "-sp") == 0) {
      float rv= strtof(argv[i+1], NULL);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, rv);
      i += 1;
    }
    if(strcmp(argv[i], "-pl") == 0) {
      float x = strtof(argv[i+1], NULL);
      float y = strtof(argv[i+2], NULL);
      float z = strtof(argv[i+3], NULL);
      float r = strtof(argv[i+4], NULL);
      float g = strtof(argv[i+5], NULL);
      float b = strtof(argv[i+6], NULL);
      float lightColors[] = {r,g,b,1};
      float lightPos[] = {x,y,z,1};
      int currLight = GL_LIGHT0+totalLights;
      glEnable(currLight);
      glLightfv(currLight, GL_AMBIENT, lightColors);
      glLightfv(currLight, GL_DIFFUSE, lightColors);
      glLightfv(currLight, GL_SPECULAR, lightColors);
      glLightfv(currLight, GL_POSITION, lightPos);
      totalLights += 1;
      numPl += 1;
      i += 6;
    }
    if(strcmp(argv[i], "-dl") == 0) {
      std::cout << typeid(GL_LIGHT0).name() << '\n';
      float x = strtof(argv[i+1], NULL);
      float y = strtof(argv[i+2], NULL);
      float z = strtof(argv[i+3], NULL);
      float r = strtof(argv[i+4], NULL);
      float g = strtof(argv[i+5], NULL);
      float b = strtof(argv[i+6], NULL);

      float lightColors[] = {r,g,b,1};
      // int currLight = pl[numDl];
      float lightPos[] = {x,y,z,0};
      int currLight = GL_LIGHT0+totalLights;
      glEnable(currLight);
      glLightfv(currLight, GL_AMBIENT, lightColors);
      glLightfv(currLight, GL_DIFFUSE, lightColors);
      glLightfv(currLight, GL_SPECULAR, lightColors);
      glLightfv(currLight, GL_POSITION, lightPos);
      totalLights += 1;
      // numDl += 1;
      i += 6;
    }
  }
}



//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, int z, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glNormal3f(0, 0, z);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Draw a filled circle.  
//****************************************************


void circle(float centerX, float centerY, float radius) {
  // Draw inner circle
  glBegin(GL_POINTS);

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

  int i,j;  // Pixel indices

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));



  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      if (dist<=radius) {

        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

        setPixel(i, j, z, 0.0, 0.0, 0.0);

        // This is amusing, but it assumes negative color values are treated reasonably.
        // setPixel(i,j, x/radius, y/radius, z/radius );
      }


    }
  }


  glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);       // clear the color buffer

  glMatrixMode(GL_MODELVIEW);             // indicate we are specifying camera transformations
  glLoadIdentity();               // make sure transformation is "zero'd"


  // Start drawing

  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) * 0.45);

  glFlush();
  glutSwapBuffers();          // swap buffers (we earlier set double buffer)
}


void handle(unsigned char key, int x, int y) {
  switch (key) {
    case 32: //space
      exit(0);
      break;
  }
  glutPostRedisplay();
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);
  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene(argc, argv);              // quick function to set up scene
  glutDisplayFunc(myDisplay);       // function to run when its time to draw something
  glutReshapeFunc(myReshape);       // function to run when the window gets resized
  glutKeyboardFunc(handle); //exit on space
  glutMainLoop();             // infinite loop that will keep drawing and resizing
  // and whatever else

  return 0;
}