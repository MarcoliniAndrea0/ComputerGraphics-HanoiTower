#include "engine.h"
#include "definitions.h"
#include "hanoi.h"
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

int main(int argc, char *argv[])
{
    // Credits:
    std::cout << "Tower of Hanoi Game" << std::endl;
    std::cout << "Graphics Engine Client" << std::endl;
    std::cout << std::endl;

    // Parse command line arguments
    int windowWidth = 1024;
    int windowHeight = 768;
    std::string windowTitle = "Tower of Hanoi";
    
    if (argc >= 4) {
        windowWidth = std::atoi(argv[1]);
        windowHeight = std::atoi(argv[2]);
        windowTitle = argv[3];
    } else {
        std::cout << "Usage: " << argv[0] << " <width> <height> <title>" << std::endl;
        std::cout << "Using default: " << windowWidth << "x" << windowHeight 
                  << " \"" << windowTitle << "\"" << std::endl;
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
        
        // Set background color
        Engine::setBackGround(0.1f, 0.1f, 0.15f); // Dark blue background

        std::shared_ptr<Node> scenePtr = OVOParser::fromFile("./client/scena1.ovo");
        Engine::setScene(scenePtr);

        std::shared_ptr<PerspectiveCamera> camera =  std::make_shared<PerspectiveCamera>();
        camera->setName("Main");
        camera->setBaseMatrix(glm::mat4(1.0f));
        camera->setFarClipping(1000.0f);
        camera->setNearClipping(0.1f);
        camera->setFov(60.0f);
        camera->setWindowSize(windowWidth, windowHeight);
        //camera->setPosition(glm::vec3(-0.3f,-0.532f,1.5f));
        camera->setPosition(glm::vec3(0.245f,-1.585f,1.608f));
        camera->setRotation(glm::vec3(-25.0f,0.0f,0.0f));

        scenePtr->addChild(camera);
        camera->setActive(true);
        
        Engine::setActiveCamera(camera);
        
        // Initialize Hanoi game
        HanoiGame::init(scenePtr);
        
        std::cout << "\n=== Tower of Hanoi ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "- Click: Select disk / Move to tower" << std::endl;
        std::cout << "- R: Reset game" << std::endl;
        std::cout << "- H: Show help" << std::endl;
        std::cout << "- ESC: Exit" << std::endl;
        std::cout << "=====================" << std::endl;
        
        // Set initial screen text
        Engine::setScreenText(HanoiGame::getStatus());
        
        // Main loop
        std::cout << "\nStarting game..." << std::endl;
        
        while (Engine::isRunning()) {
            Engine::clearScreen();
            Engine::update();
            Engine::render();
            Engine::swapBuffers();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
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
        return;
    }
    
    // Handle Hanoi game keys
    HanoiGame::handleKey(key);
    
    // Update screen text with game status
    Engine::setScreenText(HanoiGame::getStatus());
}

void specialCallback(int key, int mouseX, int mouseY) {
    std::string keyName;
    switch(key) {
        case ENG_KEY_F1: keyName = "F1"; break;
        case ENG_KEY_LEFT: keyName = "LEFT ARROW"; break;
        case ENG_KEY_RIGHT: keyName = "RIGHT ARROW"; break;
        case ENG_KEY_UP: keyName = "UP ARROW"; break;
        case ENG_KEY_DOWN: keyName = "DOWN ARROW"; break;
        default: keyName = "UNKNOWN"; break;
    }
    
    std::cout << "Special key pressed: " << keyName 
              << " at (" << mouseX << ", " << mouseY << ")" << std::endl;
              
    // F1 for help
    if (key == ENG_KEY_F1) {
        HanoiGame::showHelp();
    }
    
    // Arrow keys could be used for camera control
    if (key == ENG_KEY_UP) {
        Engine::setScreenText("UP arrow - Camera control not implemented");
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
    
    // Left click for Hanoi game interaction
    if (button == ENG_LEFT_BUTTON && state == ENG_DOWN) {
        HanoiGame::handleClick(mouseX, mouseY);
        
        // Update screen text
        Engine::setScreenText(HanoiGame::getStatus());
    }
    
    // Right click for camera reset or other functions
    if (button == ENG_RIGHT_BUTTON && state == ENG_DOWN) {
        std::cout << "Right click - could be used for camera reset" << std::endl;
        Engine::setScreenText("Right click - Feature not implemented");
    }
}

void blinkingCallback() {
    static int blinkCount = 0;
    blinkCount++;
    
    // Example: Update game animation
    HanoiGame::update(0.05f); // Assuming ~20 FPS for blinking
    
    // Optional: print blink count occasionally
    if (blinkCount % 100 == 0) {
        std::cout << "Blink callback called " << blinkCount << " times" << std::endl;
        
        // Update screen text if game is won
        if (HanoiGame::isGameWon()) {
            Engine::setScreenText("YOU WIN! Moves: " + 
                                 std::to_string(HanoiGame::getMoveCount()) + 
                                 "/" + std::to_string(HanoiGame::getMinMoves()));
        }
    }
}