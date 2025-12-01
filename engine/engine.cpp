/**
 * @file		engine.cpp
 * @brief	Graphics engine main file
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch] << change this to your group members
 */



//////////////
// #INCLUDE //
//////////////

// Main include:
#include "engine.h"

// C/C++:
#include <iostream>
//#include <source_location>
#include <ctime>



/////////////////////////
// RESERVED STRUCTURES //
/////////////////////////

/**
 * @brief Base class reserved structure (using PIMPL/Bridge design pattern https://en.wikipedia.org/wiki/Opaque_pointer).
 */
struct Eng::Base::Reserved
{
   // Flags:
   bool initFlag;

   /**
    * Constructor.
    */
   Reserved() : initFlag{false}
   {
   }
};



////////////////////////
// BODY OF CLASS Base //
////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor.
 */
ENG_API Eng::Base::Base() //: reserved(std::make_unique<Eng::Base::Reserved>())
    : reserved(std::unique_ptr<Eng::Base::Reserved>(new Eng::Base::Reserved()))
{
#ifdef _DEBUG
   //std::cout << "[+] " << std::source_location::current().function_name() << " invoked" << std::endl;
    std::cout << "[+] Engine Base Constructor invoked" << std::endl;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Destructor.
 */
ENG_API Eng::Base::~Base()
{
#ifdef _DEBUG
   //std::cout << "[-] " << std::source_location::current().function_name() << " invoked" << std::endl;
   std::cout << "[-] Engine Base Destructor invoked" << std::endl;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Gets a reference to the (unique) singleton instance.
 * @return reference to singleton instance
 */
Eng::Base ENG_API &Eng::Base::getInstance()
{
   static Base instance;
   return instance;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Init internal components.
 * @return TF
 */
bool ENG_API Eng::Base::init(std::string windowName, int width, int height, int argc, char *argv[])
{
   // Already initialized?
   if (reserved->initFlag)
   {
      std::cout << "ERROR: engine already initialized" << std::endl;
      return false;
   }

   // init random (if needed)
   srand(time(NULL));

   // FreeGLUT can parse command-line params, in case:
   glutInit(&argc, argv);

   int screen_width = glutGet(GLUT_SCREEN_WIDTH);
   int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

   // Calculate centered position
   int window_x = (screen_width - width) / 2;
   int window_y = (screen_height - height) / 2;
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowPosition(window_x, window_y);
   glutInitWindowSize(width, height);

   // Set some optional flags:
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                 GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   // Create the window with a specific title:
   setWindowId(glutCreateWindow((const char*)&windowName));

   // z buffer
   glEnable(GL_DEPTH_TEST);

   // render vertices counterclockwise
   glFrontFace(GL_CCW);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // back face culling
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // normal vector normalization
   glEnable(GL_NORMALIZE);

   // enable lighting
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   // get max no. of lights
   GLint maxNrOfLights;
   glGetIntegerv(GL_MAX_LIGHTS, &maxNrOfLights);

   // The OpenGL context is now initialized...

   // Set callback functions:
   //glutDisplayFunc(displayCallback);
   //glutReshapeFunc(reshapeCallback);

   // Done:
   std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
   reserved->initFlag = true;
   return true;
}

void ENG_API Eng::Base::run() {
   glutMainLoop();
}

int ENG_API Eng::Base::getWindowId()
{
   return this->windowId;
}

void ENG_API Eng::Base::setWindowId(int id)
{
   this->windowId = id;
}

void ENG_API Eng::Base::displayCallback()
{
   //clearWindow();
}

void ENG_API Eng::Base::setKeyboardCallback(void (*callback)(unsigned char, int, int))
{
   glutKeyboardFunc(callback);
   glutPostWindowRedisplay(windowId);
}

void ENG_API Eng::Base::setSpecialCallback(void (*callback)(unsigned char, int, int))
{
   glutKeyboardFunc(callback);
   glutPostWindowRedisplay(windowId);
}


void ENG_API Eng::Base::reshapeCallback(int width, int height)
{
   // temporary code, to be changed with actual camera information
    std::cout << "[reshape func invoked]" << std::endl;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)width / (float)height, 1.0f, 1000.0f);

    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
}

/**
 * Clears the buffer
 */
void ENG_API Eng::Base::clearWindow()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Swaps the buffers
 */
void ENG_API Eng::Base::swapBuffer()
{
   glutSwapBuffers();
}

void ENG_API Eng::Base::drawSolidCube(float size)
{
   glutSolidCube(size);
}

void ENG_API Eng::Base::drawSolidSphere(float radius, GLint slices, GLint stacks)
{
   glutSolidSphere(radius, slices, stacks);
}

void ENG_API Eng::Base::drawSolidTorus(float outerRadius, float innerRadius, GLint side, GLint rings)
{
   glutSolidTorus(outerRadius, innerRadius, side, rings);
}

void ENG_API Eng::Base::drawSolidTeapot(float size)
{
   glutSolidTeapot(size);
}

/**
 * Test GLM functionality
 */
void ENG_API Eng::Base::testGLM()
{
   glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10));
   glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
   glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

   glm::mat4 matrix = translation * rotation * scale;

   std::cout << "GLM OK" << std::endl;
}

void ENG_API Eng::Base::testObjectGeneration()
{
   drawSolidCube(10.0f);
   drawSolidSphere(10.0f, 10, 10);
   drawSolidTeapot(10.0f);
   std::cout << "graphics generation OK" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Free internal components.
 * @return TF
 */
bool ENG_API Eng::Base::free()
{
   // Not initialized?
   if (!reserved->initFlag)
   {
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   // Here you can properly dispose of any allocated resource (including third-party dependencies)...

   // Done:
   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved->initFlag = false;
   return true;
}