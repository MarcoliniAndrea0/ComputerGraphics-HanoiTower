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
   #include <string>

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
#define LIB_NAME      "My Graphics Engine v0.1a (debug)"
#else
#define LIB_NAME      "My Graphics Engine v0.1a"
#endif
#define LIB_VERSION   10                         ///< Library version (divide by 10)

// Export API:
#ifdef _WINDOWS
#ifdef ENGINE_EXPORTS
#define ENG_API __declspec(dllexport)
#else
#define ENG_API __declspec(dllimport)
#endif      
#pragma warning(disable : 4251) 
#else 
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

	Base(Base const&) = delete;
	~Base();
	void operator=(Base const&) = delete;

	static Base& getInstance();

	bool init(std::string windowName, int width, int height, int argc, char* argv[]);
	bool free();
	void run();

	// --- Metodi di Test ---
	void testGLM();
	void testObjectGeneration();
	void drawSolidCube(float side);
	void drawSolidTorus(float outerRadius, float innerRadius, GLint side, GLint rings);
	void drawSolidSphere(float radius, GLint slices, GLint stacks);
	void drawSolidTeapot(float side);

	// --- Callbacks (Proxy per FreeGLUT) ---
	// Questi sono fondamentali per far funzionare FreeGLUT statico
	void setDisplayCallback(void (*callback)(void));
	void setReshapeCallback(void (*callback)(int, int));
	void setKeyboardCallback(void (*callback)(unsigned char, int, int));
	void setSpecialCallback(void (*callback)(int, int, int));

	// Window & Buffer
	void clearWindow();
	void swapBuffer();
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

