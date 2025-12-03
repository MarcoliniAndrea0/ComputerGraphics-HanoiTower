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


#include <iostream>
#include <ctime> // Per time()



/////////////////////////
// RESERVED STRUCTURES //
/////////////////////////

// Usiamo il namespace per definire la struct in modo pulito
namespace Eng {

    // Definizione della struct Reserved (PIMPL)
    struct Base::Reserved
    {
        bool initFlag;
        Reserved() : initFlag(false) {}
    };

    // --- Costruttore ---
    Base::Base()
        // FIX COMPATIBILITÀ: Usiamo new invece di make_unique
        : reserved(std::unique_ptr<Base::Reserved>(new Base::Reserved()))
    {
#ifdef _DEBUG
        std::cout << "[+] Engine Base Constructor invoked" << std::endl;
#endif
    }

    // --- Distruttore ---
    Base::~Base()
    {
#ifdef _DEBUG
        std::cout << "[-] Engine Base Destructor invoked" << std::endl;
#endif
    }

    // --- Singleton ---
    Base& Base::getInstance()
    {
        static Base instance;
        return instance;
    }

    // --- Inizializzazione ---
    bool Base::init(std::string windowName, int width, int height, int argc, char* argv[])
    {
        if (reserved->initFlag)
        {
            std::cout << "ERROR: engine already initialized" << std::endl;
            return false;
        }

        srand((unsigned int)time(NULL));

        glutInit(&argc, argv);

        int screen_width = glutGet(GLUT_SCREEN_WIDTH);
        int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

        int window_x = (screen_width - width) / 2;
        int window_y = (screen_height - height) / 2;
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowPosition(window_x, window_y);
        glutInitWindowSize(width, height);

        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        setWindowId(glutCreateWindow((const char*)&windowName));

        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_NORMALIZE);
        glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        GLint maxNrOfLights;
        glGetIntegerv(GL_MAX_LIGHTS, &maxNrOfLights);

        std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
        reserved->initFlag = true;
        return true;
    }

    bool Base::free()
    {
        if (!reserved->initFlag)
        {
            std::cout << "ERROR: engine not initialized" << std::endl;
            return false;
        }
        std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
        reserved->initFlag = false;
        return true;
    }

    void Base::run() {
        glutMainLoop();
    }

    // --- Gestione Callbacks (PROXY) ---
    void Base::setDisplayCallback(void (*callback)(void)) {
        glutDisplayFunc(callback);
    }

    void Base::setReshapeCallback(void (*callback)(int, int)) {
        glutReshapeFunc(callback);
    }

    void Base::setKeyboardCallback(void (*callback)(unsigned char, int, int)) {
        glutKeyboardFunc(callback);
        glutPostWindowRedisplay(windowId);
    }

    void Base::setSpecialCallback(void (*callback)(int, int, int)) {
        glutSpecialFunc(callback);
        glutPostWindowRedisplay(windowId);
    }

    // --- Altri Metodi ---
    int Base::getWindowId() { return this->windowId; }
    void Base::setWindowId(int id) { this->windowId = id; }

    void Base::clearWindow() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Base::swapBuffer() {
        glutSwapBuffers();
    }

    void Base::drawSolidCube(float size) { glutSolidCube(size); }
    void Base::drawSolidSphere(float radius, GLint slices, GLint stacks) { glutSolidSphere(radius, slices, stacks); }
    void Base::drawSolidTorus(float outerRadius, float innerRadius, GLint side, GLint rings) { glutSolidTorus(outerRadius, innerRadius, side, rings); }
    void Base::drawSolidTeapot(float size) { glutSolidTeapot(size); }

    void Base::testGLM() {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10));
        std::cout << "GLM OK" << std::endl;
    }

    void Base::testObjectGeneration() {
        std::cout << "graphics generation OK" << std::endl;
    }

}

/**
 * Test GLM functionality
 
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
 *
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
*/
