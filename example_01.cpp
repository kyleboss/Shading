
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

typedef struct {
  float val1;
  float val2;
  float val3;
  void Set(float v1, float v2, float v3){val1=v1, val2=v2, val3=v3;}
  float length(){return sqrt(pow(val1,2)+pow(val2,2)+pow(val3,2));}
  void normalize(){
    float length = this->length();
    this->val1 = this->val1/length;
    this->val2 = this->val2/length;
    this->val3 = this->val3/length;
  }
} Vec;

float dotProduct(Vec vec1, Vec vec2) {
  return vec1.val1*vec2.val1 + vec1.val2*vec2.val2 + vec1.val3*vec2.val3;
}

Vec mul(Vec vec1, Vec vec2) {
  Vec mulVec;
  float val1 = vec1.val1*vec2.val1;
  float val2 = vec1.val2*vec2.val2;
  float val3 = vec1.val3*vec2.val3;
  mulVec.Set(val1, val2, val3);
  return mulVec;
}

Vec sub(Vec vec1, Vec vec2) {
  Vec subVec;
  float val1 = vec1.val1-vec2.val1;
  float val2 = vec1.val2-vec2.val2;
  float val3 = vec1.val3-vec2.val3;
  subVec.Set(val1, val2, val3);
  return subVec;
}

typedef struct {
  Vec color;
  Vec pos;
  void Set(Vec setColor, Vec setPos){color=setColor, pos=setPos;}
} Light;

Vec ka;
Vec kd;
Vec ks;
int numDl = 0;
int numPl = 0;
Light dl0, dl1, dl2, dl3, dl4, pl0, pl1, pl2, pl3, pl4;
Light dl[] = {dl0, dl1, dl2, dl3, dl4};
Light pl[] = {pl0, pl1, pl2, pl3, pl4};

//****************************************************
// Simple init function
//****************************************************
void initScene(int argc, char *argv[]){
  GLint iLights;
  glGetIntegerv(GL_MAX_LIGHTS, &iLights);
  for (int i = 1; i < argc; ++i) {
    std::cout << argv[i] << std::endl;
    if(strcmp(argv[i], "-ka") == 0) {
      float r = strtof(argv[i+1], NULL);
      float g = strtof(argv[i+2], NULL);
      float b = strtof(argv[i+3], NULL);
      ka.Set(r,g,b);
      GLfloat ambient[] = {r,g,b,1};
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
      i += 3;
    }
    if(strcmp(argv[i], "-kd") == 0) {
      float r = strtof(argv[i+1], NULL);
      float g = strtof(argv[i+2], NULL);
      float b = strtof(argv[i+3], NULL);
      kd.Set(r,g,b);
      GLfloat diffuse[] = {r,g,b,1};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      i += 3;
    }
    if(strcmp(argv[i], "-ks") == 0) {
      float r = strtof(argv[i+1], NULL);
      float g = strtof(argv[i+2], NULL);
      float b = strtof(argv[i+3], NULL);
      ks.Set(r,g,b);
      GLfloat specular[] = {r,g,b,1};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      i += 3;
    }
    if(strcmp(argv[i], "-sp") == 0) {
      float rv= strtof(argv[i+1], NULL);
      glMaterialf(GL_FRONT, GL_SHININESS, rv);
      i += 1;
    }
    if(strcmp(argv[i], "-pl") == 0) {
      float x = strtof(argv[i+1], NULL);
      float y = strtof(argv[i+2], NULL);
      float z = strtof(argv[i+3], NULL);
      float r = strtof(argv[i+4], NULL);
      float g = strtof(argv[i+5], NULL);
      float b = strtof(argv[i+6], NULL);
      Vec lightColor;
      Vec lightPos;
      lightColor.Set(r,g,b);
      lightPos.Set(x,y,z);
      pl[numPl].Set(lightColor, lightPos);
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
      Vec lightColor;
      Vec lightPos;
      lightColor.Set(r,g,b);
      lightPos.Set(x,y,z);
      dl[numDl].Set(lightColor, lightPos);
      numDl += 1;
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

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
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
        
        Vec total; //treat as color

        Vec norm; //normal: position vector
        norm.Set(i, j, z);
        norm.normalize();

        float ln = dotProduct(dl[k].pos.normalize(), norm);
        
        diffpos = max(0, ln); //max(l.n, 0)
        
        Vec r;
        r.Set(norm.val1, norm.val2, norm.val3); //currently r = n
        r.Scale(2 * ln)
        r = sub(r, dl[k].pos.normalize()); //r is correct


        for (k=0; k < numDl; k++) { //loop direction light
          Vec ambient; // ambient = ka*I, color
          ambient.Set(mul(ka, dl[k].color));

          Vec diffuse; //diffuse = kd * I * max(l.n, 0), color
          diffuse.set(mul(kd, dl[k].color));


           //DEFINE  
            total += ka*i + 
        }



        setPixel(i, j, total.val1, total.val2, total.val3);

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

  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) / 3.0);

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