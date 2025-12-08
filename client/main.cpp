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

// C/C++:
#include <iostream>
#include <memory>

/////////////
// GLOBALS //
/////////////

// Callback functions
void keyboardCallback(const unsigned char key, const int mouseX, const int mouseY);
void specialCallback(int key, int mouseX, int mouseY);
void mouseCallback(int button, int state, int mouseX, int mouseY);
void blinkingCallback();

//////////
// MAIN //
//////////

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

    // Parse command line arguments (optional)
    int windowWidth = 1024;
    int windowHeight = 768;
    std::string windowTitle = "Graphics Engine Client";
    
    if (argc >= 4) {
        windowWidth = std::atoi(argv[1]);
        windowHeight = std::atoi(argv[2]);
        windowTitle = argv[3];
    } else {
        std::cout << "Usage: " << argv[0] << " <width> <height> <title>" << std::endl;
        std::cout << "Using default: " << windowWidth << "x" << windowHeight << " \"" << windowTitle << "\"" << std::endl;
    }

    // Init engine:
    try {
        std::cout << "Initializing engine..." << std::endl;
        Engine::init(windowTitle, windowWidth, windowHeight);
        
        // Set callbacks
        Engine::setKeyboardCallback(keyboardCallback);
        Engine::setMethodSpecialCallback(specialCallback);
        Engine::setMouseCallback(mouseCallback);
        Engine::setBlinkingCallback(blinkingCallback);
        
        // Set background color (light gray)
        Engine::setBackGround(0.8f, 0.8f, 0.9f);
        
        // Set screen text
        Engine::setScreenText("Engine initialized successfully!");
        
        std::cout << "Engine initialized successfully!" << std::endl;
        std::cout << "Window: " << windowWidth << "x" << windowHeight << std::endl;
        
        // test components - create a simple scene
        std::cout << "\nCreating test scene..." << std::endl;
        
        // You can add your test components here
        // For example:
        // auto testNode = std::make_shared<Node>("TestNode");
        // Engine::setScene(testNode);
        
        // Main loop
        std::cout << "\nEntering main loop..." << std::endl;
        std::cout << "Press ESC to exit" << std::endl;
        std::cout << "------------------------" << std::endl;
        
        while (Engine::isRunning()) {
            Engine::clearScreen();
            Engine::update();
            Engine::render();
            Engine::swapBuffers();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error initializing engine: " << e.what() << std::endl;
        return 1;
    }

    // Release engine:
    std::cout << "\nShutting down engine..." << std::endl;
    Engine::quit();
    
    // Done:
    std::cout << "\n[application terminated]" << std::endl;
    return 0;
}

//////////////////
// CALLBACKS    //
//////////////////

void keyboardCallback(const unsigned char key, const int mouseX, const int mouseY) {
    std::cout << "Key pressed: '" << key << "' (ASCII: " << (int)key 
              << ") at (" << mouseX << ", " << mouseY << ")" << std::endl;
    
    // ESC key to exit
    if (key == 27) {  // ESC key
        std::cout << "ESC pressed - stopping engine..." << std::endl;
        Engine::stop();
    }
    
    // Space bar example
    if (key == ' ') {
        std::cout << "Space bar pressed!" << std::endl;
        Engine::setScreenText("Space bar pressed!");
    }
    
    // 'R' key example - change background to red
    if (key == 'r' || key == 'R') {
        Engine::setBackGround(1.0f, 0.0f, 0.0f);
        std::cout << "Background changed to red" << std::endl;
    }
    
    // 'G' key example - change background to green
    if (key == 'g' || key == 'G') {
        Engine::setBackGround(0.0f, 1.0f, 0.0f);
        std::cout << "Background changed to green" << std::endl;
    }
    
    // 'B' key example - change background to blue
    if (key == 'b' || key == 'B') {
        Engine::setBackGround(0.0f, 0.0f, 1.0f);
        std::cout << "Background changed to blue" << std::endl;
    }
    
    // 'W' key example - print scene info
    if (key == 'w' || key == 'W') {
        auto scene = Engine::getScene();
        auto camera = Engine::getActiveCamera();
        
        if (scene) {
            std::cout << "Scene has " << /* scene->getChildCount() */ " children" << std::endl;
        } else {
            std::cout << "No scene set" << std::endl;
        }
        
        if (camera) {
            std::cout << "Active camera: " << camera->getName() << std::endl;
        } else {
            std::cout << "No active camera" << std::endl;
        }
    }
}

void specialCallback(int key, int mouseX, int mouseY) {
    std::string keyName;
    switch(key) {
        case ENG_KEY_F1: keyName = "F1"; break;
        case ENG_KEY_F2: keyName = "F2"; break;
        case ENG_KEY_F3: keyName = "F3"; break;
        case ENG_KEY_F4: keyName = "F4"; break;
        case ENG_KEY_F5: keyName = "F5"; break;
        case ENG_KEY_F6: keyName = "F6"; break;
        case ENG_KEY_F7: keyName = "F7"; break;
        case ENG_KEY_F8: keyName = "F8"; break;
        case ENG_KEY_F9: keyName = "F9"; break;
        case ENG_KEY_F10: keyName = "F10"; break;
        case ENG_KEY_F11: keyName = "F11"; break;
        case ENG_KEY_F12: keyName = "F12"; break;
        case ENG_KEY_LEFT: keyName = "LEFT ARROW"; break;
        case ENG_KEY_RIGHT: keyName = "RIGHT ARROW"; break;
        case ENG_KEY_UP: keyName = "UP ARROW"; break;
        case ENG_KEY_DOWN: keyName = "DOWN ARROW"; break;
        case ENG_KEY_PAGE_UP: keyName = "PAGE UP"; break;
        case ENG_KEY_PAGE_DOWN: keyName = "PAGE DOWN"; break;
        case ENG_KEY_HOME: keyName = "HOME"; break;
        case ENG_KEY_END: keyName = "END"; break;
        case ENG_KEY_INSERT: keyName = "INSERT"; break;
        default: keyName = "UNKNOWN"; break;
    }
    
    std::cout << "Special key pressed: " << keyName 
              << " at (" << mouseX << ", " << mouseY << ")" << std::endl;
              
    // Arrow keys example for camera control
    if (key == ENG_KEY_UP) {
        Engine::setScreenText("UP arrow pressed");
    }
    if (key == ENG_KEY_DOWN) {
        Engine::setScreenText("DOWN arrow pressed");
    }
    if (key == ENG_KEY_LEFT) {
        Engine::setScreenText("LEFT arrow pressed");
    }
    if (key == ENG_KEY_RIGHT) {
        Engine::setScreenText("RIGHT arrow pressed");
    }
}

void mouseCallback(int button, int state, int mouseX, int mouseY) {
    std::string buttonName;
    std::string stateName = (state == ENG_DOWN) ? "DOWN" : "UP";
    
    switch(button) {
        case ENG_LEFT_BUTTON: buttonName = "LEFT"; break;
        case ENG_MIDDLE_BUTTON: buttonName = "MIDDLE"; break;
        case ENG_RIGHT_BUTTON: buttonName = "RIGHT"; break;
        default: buttonName = "UNKNOWN"; break;
    }
    
    std::cout << "Mouse " << buttonName << " button " << stateName 
              << " at (" << mouseX << ", " << mouseY << ")" << std::endl;
    
    // Left click example - select object
    if (button == ENG_LEFT_BUTTON && state == ENG_DOWN) {
        auto selectedNode = Engine::getNodeByClick(mouseX, mouseY);
        if (selectedNode) {
            std::cout << "Selected node: " << selectedNode->getName() << std::endl;
            Engine::setScreenText("Selected: " + selectedNode->getName());
        } else {
            std::cout << "No node selected at click position" << std::endl;
            Engine::setScreenText("Click: (" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")");
        }
    }
    
    // Right click example
    if (button == ENG_RIGHT_BUTTON && state == ENG_DOWN) {
        std::cout << "Right click at (" << mouseX << ", " << mouseY << ")" << std::endl;
        Engine::setScreenText("Right click");
    }
}

void blinkingCallback() {
    static int blinkCount = 0;
    blinkCount++;
    
    // Example: change screen text every few blinks
    if (blinkCount % 10 == 0) {
        Engine::setScreenText("Blink count: " + std::to_string(blinkCount));
    }
    
    // Optional: print blink count occasionally
    if (blinkCount % 100 == 0) {
        std::cout << "Blink callback called " << blinkCount << " times" << std::endl;
    }
}