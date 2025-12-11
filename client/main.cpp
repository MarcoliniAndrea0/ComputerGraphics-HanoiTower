#include <iostream>
#include <memory>
#include <engine.h>
#include <Node.h>
#include "hanoi.h"
#include <PerspectiveCamera.h>
#include <OvoParser.h>
#include <PointLight.h>
#include <Material.h>
#include <algorithm>
//#include <GL/freeglut.h>


namespace Constants {
    constexpr inline int MOUSE_LEFT_BUTTON = 0;
    constexpr inline int MOUSE_DOWN = 0;
    constexpr inline int KEYBOARD_KEY_UP = 101;
    constexpr inline int KEYBOARD_KEY_DOWN = 103;
    constexpr inline int KEYBOARD_KEY_LEFT = 100;
    constexpr inline int KEYBOARD_KEY_RIGHT = 102;
    constexpr inline int KEYBOARD_KEY_ENTER = 13;
}

// Variabili globali per la gestione della scena e del tempo
std::shared_ptr<Node> rootNode = nullptr;
int lastTime = 0;

// Camera reference
std::shared_ptr<PerspectiveCamera> whiteCamera;

std::shared_ptr<PerspectiveCamera> freeCamera;

std::shared_ptr<PerspectiveCamera> currentActiveCamera = whiteCamera;


// Movement speed
float cameraSpeed = 150.0f;
float lightSpeed = 5.0f;
bool isLightEnabled = false;

// Rotation speed
float cameraRotationSpeed = 5.0f;

/*
// --- Callback del Mouse per la logica di gioco ---
void mouseCallback(int button, int state, int x, int y) {
    // Passa il click sinistro alla logica della Torre di Hanoi
    if (button == Constants::MOUSE_LEFT_BUTTON && state == Constants::MOUSE_DOWN) {
        HanoiGame::handleClick(x, y);
    }
}
*/

std::string getInstructions()
{
    std::stringstream text;
    text << "---ENVIRONMENT COMMANDS---\n";
    text << "[l] - Turn on/off light\n";
    text << "[c] - Switch camera\n";
    text << "[r] - Reset Scene\n";
    text << "Free camera commands:\n";
    text << "   [w][a][s][d] - Move camera\n";
    text << "   [q][e][y][x] - Rotate camera\n";
    text << "\n---LIGHT CONTROL---\n";
    text << "[i][k] - Move Forward/Back\n";
    text << "[j][h] - Move Left/Right\n";
    text << "[u][o] - Move Up/Down\n";
    text << "\n---GAME---\n";
    text << "[p] - Help game\n";
    //text << "[LeftMouseClick] - Select Disk\n";
    //text << "[LeftMouseClick] - Select Tower\n";
    text << "[1][2][3] - Select/Move to Tower\n";
    text << "[v] - Undo\n";
    text << "[b] - Redo\n";
    //Engine::setScreenText(text.str());

    return text.str();

}



void intializeAndSetCameras(std::shared_ptr<Node> scene)
{
    //white camera
    whiteCamera = std::make_shared<PerspectiveCamera>();
    whiteCamera->setName("WhiteCamera");
    whiteCamera->setFarClipping(5000.0f);
    whiteCamera->setNearClipping(0.1f);
    whiteCamera->setPosition(glm::vec3(-700.245f, 1001.608f, 551.585f));
    whiteCamera->setRotation(glm::vec3(70.0f, 90.0f, 70.0f));
    scene->addChild(whiteCamera);
    Engine::setActiveCamera(whiteCamera);

    //free camera
    freeCamera = std::make_shared<PerspectiveCamera>();
    freeCamera->setName("BlackCamera");
    freeCamera->setFarClipping(5000.0f);
    freeCamera->setNearClipping(0.1f);
    freeCamera->setPosition(glm::vec3(400.0f, 1200.0f, 550.0f));
    freeCamera->setRotation(glm::vec3(70.0f, 90.0f, 70.0f));
    scene->addChild(freeCamera);
    Engine::setActiveCamera(freeCamera);

}

void resetScene() {
    // Ottieni la scena corrente
    std::shared_ptr<Node> scene = Engine::getScene();

    // Rimuovi tutti i figli della scena (inclusi camera e luce)
    scene->removeAllChildren();

    // Riaggiungi la camera prospettica
    intializeAndSetCameras(scene);

    // Ricarica la scena dal file OVO
    std::shared_ptr<Node> ovoScene = OVOParser::fromFile("./scena1.ovo");
    if (ovoScene) {
        scene->addChild(ovoScene);
        std::cout << "[Info] Scene successfully reset." << std::endl;
        //Reinizializza la logica della Torre di Hanoi
        HanoiGame::init(scene);
        HanoiGame::showHelp();
    }
    else {
        std::cerr << "[Error] Unable to reload OVO file." << std::endl;
    }
}

void switchLight()
{
    // 1. Trova il nodo della luce/lampada
    std::shared_ptr<Node> objectNode = Engine::findObjectByName("Spot001");

    if (objectNode) {
        // Cast ai tipi specifici
        std::shared_ptr<SpotLight> spotlight = std::dynamic_pointer_cast<SpotLight>(objectNode);
        std::shared_ptr<Mesh> lampMesh = std::dynamic_pointer_cast<Mesh>(objectNode);

        // Variabili statiche per salvare lo stato della luce
        // Salviamo Diffuse e Specular perché sono quelle che illuminano la scena
        static glm::vec3 savedDiffuse = glm::vec3(1.0f);
        static glm::vec3 savedSpecular = glm::vec3(1.0f);
        static float savedRadius = 200.0f; // Salviamo anche il raggio originale
        static bool hasSavedState = false;

        if (spotlight) {

            // Salva lo stato originale solo la prima volta (quando è sicuramente accesa)
            if (!hasSavedState && isLightEnabled) {
                savedDiffuse = spotlight->getDiffuseColor();   // Metodo corretto da light.h
                savedSpecular = spotlight->getSpecularColor(); // Metodo corretto da light.h
                // Nota: non c'è getRadius() pubblico in SpotLight.h che vedo, quindi assumiamo 200 o lo hardcodiamo
                // Se hai aggiunto getRadius() nel .h, usa quello. Altrimenti usa il valore noto (es. 200).
                hasSavedState = true;
            }

            if (isLightEnabled) {
                // --- SPEGNIMENTO ---

                // 1. Spegni la luce impostando i colori a nero (vec3(0.0f))
                spotlight->setDiffuseColor(glm::vec3(0.0f));
                spotlight->setSpecularColor(glm::vec3(0.0f));
                spotlight->setAmbientColor(glm::vec3(0.0f)); // Spegniamo anche l'ambiente per sicurezza

                // 2. Raggio a 0
                spotlight->setRadius(0.0f);

                // 3. Feedback visivo sulla lampadina (Mesh)
                if (lampMesh) {
                    // Nota: I metodi set...Color in material.h prendono glm::vec3, non vec4!
                    lampMesh->getMaterial()->setAmbientColor(glm::vec3(0.1f, 0.1f, 0.1f));
                    lampMesh->getMaterial()->setDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));

                    // CORREZIONE QUI: Usa setEmissionColor
                    lampMesh->getMaterial()->setEmissionColor(glm::vec3(0.0f, 0.0f, 0.0f));
                }

                isLightEnabled = false;
                std::cout << "[Info] Spotlight SPENTA." << std::endl;
            }
            else {
                // --- ACCENSIONE ---

                // 1. Ripristina i colori salvati
                spotlight->setDiffuseColor(savedDiffuse);
                spotlight->setSpecularColor(savedSpecular);
                // L'ambientale della luce spesso è basso o nullo, ripristina un valore basso o quello salvato se lo gestisci
                spotlight->setAmbientColor(glm::vec3(0.1f));

                // 2. Ripristina il raggio
                spotlight->setRadius(savedRadius);

                // 3. Feedback visivo sulla lampadina
                if (lampMesh) {
                    lampMesh->getMaterial()->setAmbientColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Colore lampada spenta ma visibile
                    lampMesh->getMaterial()->setDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));

                    // La lampadina "brilla" di nuovo (simula la luce che esce dalla mesh stessa)
                    lampMesh->getMaterial()->setEmissionColor(savedDiffuse);
                }

                isLightEnabled = true;
                std::cout << "[Info] Spotlight ACCESA." << std::endl;
            }
        }
        else {
            std::cerr << "[Error] L'oggetto 'Spot001' non è una SpotLight." << std::endl;
        }
    }
    else {
        std::cerr << "[Error] Nodo 'Spot001' non trovato." << std::endl;
    }
}

void nextCamera() {
    // Passa alla prossima camera nell'ordine in loop
    if (currentActiveCamera == whiteCamera) {
        currentActiveCamera = freeCamera;
        Engine::setActiveCamera(freeCamera);
        std::cout << "[Info] Switched to Black Camera." << std::endl;
    }
    else {
        currentActiveCamera = whiteCamera;
        Engine::setActiveCamera(whiteCamera);
        std::cout << "[Info] Switched to White Camera." << std::endl;
    }
}


void moveLight(glm::vec3 direction)
{
    // Trova il nodo della luce
    std::shared_ptr<Node> spotlightNode = Engine::findObjectByName("Spot001");

    if (spotlightNode) {
        // Ottieni la posizione corrente
        glm::vec3 currentPos = spotlightNode->getPosition();

        // Calcola la nuova posizione
        glm::vec3 newPos = currentPos + (direction * lightSpeed);

        // Applica la nuova posizione
        spotlightNode->setPosition(newPos);

        // Debug log
        std::cout << "[Info] Light Pos: " << newPos.x << ", " << newPos.y << ", " << newPos.z << std::endl;
    }
}

int main() {
    // Inizializza il motore con titolo finestra, larghezza e altezza
    Engine::init("Test Scene", 1000, 800);

    //Engine::setMouseCallback(mouseCallback);
    
    Engine::setKeyboardCallback([](const unsigned char key, const int mouseX, const int mouseY) {

        glm::mat4 globalTransform = Engine::getGlobalTransform(freeCamera);
        glm::vec3 rotation = freeCamera->getRotation();
        // Estrai i vettori front, right e up dalla matrice globale
        glm::vec3 cameraFront = glm::normalize(glm::vec3(globalTransform[2])); // Z
        glm::vec3 cameraRight = glm::normalize(glm::vec3(globalTransform[0])); // X
        glm::vec3 cameraUp = glm::normalize(glm::vec3(globalTransform[1]));    // Y

        glm::vec3 cameraPosition = freeCamera->getPosition();

        switch (key) {
        case '1':
        case '2':
        case '3':
            HanoiGame::handleKey(key);
            break;
        case 'r': // Tasto 'r' per resettare la scena
            resetScene(); //resetta anche HanoiGame
            break;
        case 'p': // Help gioco
            HanoiGame::showHelp();
            break;
        case 'c':
            nextCamera();
            break;
        case 'l':
            switchLight();
            break;
        case 'w': // Muove la camera in avanti
            cameraPosition -= cameraFront * cameraSpeed;
            break;
        case 's': // Muove la camera indietro
            cameraPosition += cameraFront * cameraSpeed;
            break;
        case 'a': // Muove la camera a sinistra
            cameraPosition -= cameraRight * cameraSpeed;
            break;
        case 'd': // Muove la camera a destra
            cameraPosition += cameraRight * cameraSpeed;
            break;
        case 'x': // Freccia su
            rotation.x -= cameraRotationSpeed;
            break;
        case 'y': // Freccia giù
            rotation.x += cameraRotationSpeed;
            break;
        case 'e': // Freccia sinistra
            rotation.z = 0;
            rotation.x = 0;
            rotation.y -= cameraRotationSpeed;
            break;
        case 'q': // Freccia destra
            rotation.z = 0;
            rotation.x = 0;
            rotation.y += cameraRotationSpeed;
            break;
        case 'i': // LUCE SPOT Avanti (-Z)
            moveLight(glm::vec3(0.0f, 0.0f, -1.0f));
            break;
        case 'k': // LUCE SPOT Indietro (+Z)
            moveLight(glm::vec3(0.0f, 0.0f, 1.0f));
            break;
        case 'j': // LUCE SPOT Sinistra (-X)
            moveLight(glm::vec3(-1.0f, 0.0f, 0.0f));
            break;
        case 'h': // LUCE SPOT Destra (+X)
            moveLight(glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 'u': // LUCE SPOT Su (+Y)
            moveLight(glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 'o': // LUCE SPOT Giù (-Y)
            moveLight(glm::vec3(0.0f, -1.0f, 0.0f));
            break;
        case 'v': // Undo:
            HanoiGame::undo();
            break;
        case 'b': // Redo
            HanoiGame::redo();
            break;
        case 27: // ESC per uscire
            Engine::stop();
            break;
        }

        freeCamera->setRotation(rotation);

        // Aggiorna la posizione della camera
        freeCamera->setPosition(cameraPosition);

        });

    // Crea un nodo di scena e lo imposta
    std::shared_ptr<Node> scene = std::make_shared<Node>();
    scene->setName("RootNode");
    Engine::setScene(scene);

    // Crea una telecamera prospettica e la imposta come attiva
    intializeAndSetCameras(scene);


    // Carica una scena da file OVO e la imposta
    std::shared_ptr<Node> ovoScene = OVOParser::fromFile("./scena1.ovo");
    if (ovoScene) {
        scene->addChild(ovoScene);

        //Inizializza logica di gioco
        //Viene passata le scena perché i dischi sono i Disk_n sono dentro il .ovo
        HanoiGame::init(scene);
        HanoiGame::showHelp();
    }
    else {
        std::cerr << "[Error] Unable to load OVO file." << std::endl;
    }

    std::shared_ptr<Node> spotlightNode = Engine::findObjectByName("Spot001");
    if (spotlightNode) {
        std::shared_ptr<SpotLight> spotlight = std::dynamic_pointer_cast<SpotLight>(spotlightNode);
        if (spotlight) spotlight->setRadius(0);
    }

    // Inizializza tempo
    //lastTime = glutGet(GLUT_ELAPSED_TIME);

    // Salva le istruzioni in una stringa fissa per non ricrearla ogni frame
    std::string staticInstructions = getInstructions();

    // Esegui il ciclo principale del motore finché non viene chiuso
    while (Engine::isRunning()) {

        // Calcolo Delta Time
        /*
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        */
        // Aggiorna logica gioco (animazioni)
        HanoiGame::update(/*deltaTime*/);

        std::string fullText = staticInstructions + "\n---STATUS---\n" + HanoiGame::getStatus();
        Engine::setScreenText(fullText);

        Engine::update();       // Gestisce eventi e callback
        Engine::clearScreen();  // Pulisce lo schermo per il nuovo frame
        Engine::render();    // Renderizza la scena

        Engine::swapBuffers();  // Scambia i buffer per visualizzare il 
    }

    // Chiudi il motore e libera le risorse
    Engine::quit();

    return 0;
}
