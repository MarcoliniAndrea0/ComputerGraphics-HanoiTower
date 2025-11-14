/**
 * @file		engine.h
 * @brief	Graphics engine main include file
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch] << change this to your group members
 */
#pragma once


 
//////////////
// #INCLUDE //
//////////////

   // C/C++:         
   #include <memory>

   // graphics
   #include <glm/glm.hpp>
   #include <glm/gtc/matrix_transform.hpp>
   #include <glm/gtc/type_ptr.hpp>
   #include <GL/freeglut.h>


/////////////
// VERSION //
/////////////

   // Generic info:
#ifdef _DEBUG
   #define LIB_NAME      "My Graphics Engine v0.1a (debug)"   ///< Library credits
#else
   #define LIB_NAME      "My Graphics Engine v0.1a"   ///< Library credits
#endif
   #define LIB_VERSION   10                           ///< Library version (divide by 10)

   // Export API:
#ifdef _WINDOWS
   // Specifies i/o linkage (VC++ spec):
   #ifdef ENGINE_EXPORTS
      #define ENG_API __declspec(dllexport)
   #else
      #define ENG_API __declspec(dllimport)
   #endif      

   // Get rid of annoying warnings:
   #pragma warning(disable : 4251) 
#else // Under linux
   #define ENG_API
#endif



///////////////
// NAMESPACE //
///////////////

namespace Eng {



//////////////
// #INCLUDE //
//////////////   

   // You can subinclude here other headers of your engine...



///////////////////////
// MAIN ENGINE CLASS //
///////////////////////

/**
 * @brief Base engine main class. This class is a singleton.
 */
class ENG_API Base final
{
//////////
public: //
//////////	      

   // Const/dest:
   Base(Base const &) = delete;
   ~Base();

   // Operators:
   void operator=(Base const &) = delete;

   // Singleton:
   static Base &getInstance();

   // Init/free:
   bool init(std::string windowName, int width, int height, int argc, char *argv[]);
   bool free();

   // start
   void run();

   // test methods
   void testGLM();
   void testObjectGeneration();

   // Create basic shapes
   void drawSolidCube(float side);
   void drawSolidTorus(float outerRadius, float innerRadius, GLint side, GLint rings);
   void drawSolidSphere(float radius, GLint slices, GLint stacks);
   void drawSolidTeapot(float side);

   // callbacks
   void displayCallback();
   void reshapeCallback(int width, int height);
   void setKeyboardCallback(void (*callback)(unsigned char, int, int));
   void setSpecialCallback(void (*callback)(unsigned char, int, int));

   // window & buffer management
   void clearWindow();
   void swapBuffer();

   // window id
   void setWindowId(int i);
   int getWindowId();


///////////
private: //
///////////	

   // Reserved:
   struct Reserved;
   std::unique_ptr<Reserved> reserved;

   // Const/dest:
   Base();

   // window ID
   int windowId;
};

}; // end of namespace Eng::

