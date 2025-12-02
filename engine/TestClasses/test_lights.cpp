#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include "../light.h"
#include "../directionalLight.h"
#include "../pointLight.h"
#include "../spotLight.h"
#include "../engine.h" 

 // Helper per stampare vettori
void printVec3(const std::string& label, const glm::vec3& v) {
    std::cout << "  " << label << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

// Helper per check booleani
void check(const std::string& testName, bool condition) {
    if (condition) std::cout << "  [OK] " << testName << std::endl;
    else std::cout << "  [FAIL] " << testName << std::endl;
}

void runLightsTest() {
    std::cout << "=== INIZIO TEST LIGHTS ===" << std::endl;

    // ---------------------------------------------------
    // 1. Test Light ID (Gerarchia Base)
    // ---------------------------------------------------
    std::cout << "\n1. Test Allocazione ID (Light Base):" << std::endl;

    // Nota: Senza un contesto OpenGL attivo, glGetIntegerv(GL_MAX_LIGHTS) potrebbe 
    // ritornare 0 o valori strani, quindi ci aspettiamo il warning nel costruttore.
    // Ma l'ID interno deve comunque incrementare.

    // Reset manuale ID (se necessario, altrimenti assumiamo parta da dove siamo)
    // Creiamo 3 luci di tipi diversi
    DirectionalLight sun("Sole");
    PointLight bulb("Lampadina");
    SpotLight torch("Torcia");

    std::cout << "  Creata 'Sole' (DirectionalLight). ID atteso: N" << std::endl;
    std::cout << "  Creata 'Lampadina' (PointLight). ID atteso: N+1" << std::endl;
    std::cout << "  Creata 'Torcia' (SpotLight). ID atteso: N+2" << std::endl;

    int idSun = sun.getLightNumber() - GL_LIGHT0;
    int idBulb = bulb.getLightNumber() - GL_LIGHT0;
    int idTorch = torch.getLightNumber() - GL_LIGHT0;

    check("ID sono sequenziali", (idBulb == idSun + 1) && (idTorch == idBulb + 1));
    std::cout << "  IDs: " << idSun << ", " << idBulb << ", " << idTorch << std::endl;


    // ---------------------------------------------------
    // 2. Test DirectionalLight
    // ---------------------------------------------------
    std::cout << "\n2. Test DirectionalLight:" << std::endl;
    glm::vec3 dir(-1.0f, -1.0f, 0.0f);
    sun.setDirection(dir);

    glm::vec3 readDir = sun.getDirection();
    check("Set/Get Direction", readDir == dir);
    printVec3("Direzione letta", readDir);


    // ---------------------------------------------------
    // 3. Test PointLight
    // ---------------------------------------------------
    std::cout << "\n3. Test PointLight:" << std::endl;
    glm::vec3 pos(10.0f, 5.0f, 10.0f);
    bulb.setPosition(pos);
    bulb.setLinearAttenuation(0.5f);

    check("Set/Get Position", bulb.getPosition() == pos);
    check("Set/Get Linear Attenuation", bulb.getLinearAttenuation() == 0.5f);
    check("Default Constant Attenuation è 1.0", bulb.getConstantAttenuation() == 1.0f);


    // ---------------------------------------------------
    // 4. Test SpotLight
    // ---------------------------------------------------
    std::cout << "\n4. Test SpotLight:" << std::endl;
    // SpotLight eredita da PointLight, quindi ha posizione
    torch.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Parametri specifici Spot
    torch.setCutoff(30.0f);
    torch.setExponent(15.0f);
    torch.setDirection(glm::vec3(0.0f, 0.0f, -1.0f));

    check("Ereditarietà (Position)", torch.getPosition() == glm::vec3(0.0f));
    check("Set/Get Cutoff", torch.getCutoff() == 30.0f);
    check("Set/Get Exponent", torch.getExponent() == 15.0f);
    check("Set/Get Spot Direction", torch.getDirection() == glm::vec3(0.0f, 0.0f, -1.0f));

    // ---------------------------------------------------
    // 5. Smoke Test Render
    // ---------------------------------------------------
    std::cout << "\n5. Smoke Test Render (No Crash Atteso):" << std::endl;
    // Anche qui, senza finestra OpenGL le chiamate glEnable/glLightfv non faranno nulla 
    // di visibile, ma verifichiamo che non crashino per puntatori nulli.
    sun.render();
    bulb.render();
    torch.render();
    check("Render methods chiamati con successo", true);

    std::cout << "\n=== FINE TEST LIGHTS ===" << std::endl;
}

int main(int argc, char* argv[]) {
    runLightsTest();
    return 0;
}