/**
 * @file		main.cpp
 * @brief	Client application (that uses the graphics engine)
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch] << change this to your group members
 */

//////////////
// #INCLUDE //
//////////////



// Library header:
#include "engine.h"
#include "definitions.h"

//#include "texture.h"
#include "material.h"
#include "light.h"
#include "directionalLight.h"
#include "prospectiveCamera.h"

// C/C++:
#include <iostream>

//////////
// MAIN //
//////////

// callbacks
void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{

   // example, actual behaviour yet to be defined
   switch (key)
   {
   case ' ':;
      break;
   case 'a':;
      break;
   case 'b':;
      break;
   case 'c':;
      break;
   case 'r':
      //   colorR = rand() % 256;
      //   colorG = rand() % 256;
      //   colorB = rand() % 256;
      break;
   default:;
      break;
   }
}

void specialCallback(unsigned char key, int mouseX, int mouseY)
{

   std::cout << "[key pressed]" << std::endl;

   switch (key)
   {
   case ENG_KEY_UP:
      ;
      break;

   case ENG_KEY_DOWN:
      ;
      break;

   case ENG_KEY_LEFT:
      ;
      break;

   case ENG_KEY_RIGHT:
      ;
      break;
   }
}

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char *argv[])
{
   // Credits:
   std::cout << "Client application example, A. Peternier (C) SUPSI" << std::endl;
   std::cout << std::endl;

   // Init engine:
   Eng::Base &eng = Eng::Base::getInstance();
   eng.init("My application", 500, 500, argc, argv);

   //Texture* myTex = new Texture("TestTex", "test.jpg"); // Sostituisci col nome del tuo file
   Material* myMat = new Material("TestMat");
   //myMat->setTexture(myTex);
   myMat->setDiffuse(glm::vec4(1.0f)); // Bianco per far risaltare la texture

   eng.setKeyboardCallback(keyboardCallback);

   // test components
   eng.testGLM();
   eng.drawSolidSphere(10,10,10);
   // eng.testObjectGeneration();

   // Release engine:
   eng.free();

   // Done:
   std::cout << "\n[application terminated]" << std::endl;
   return 0;
}
