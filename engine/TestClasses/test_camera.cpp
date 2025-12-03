#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "../prospectiveCamera.h"
#include "../engine.h"

void printMatrix(const std::string& label, const glm::mat4& m) {
    std::cout << label << ":\n";
    const float* p = glm::value_ptr(m);
    // Stampiamo solo i primi 4 valori per brevità (prima colonna)
    std::cout << "| " << p[0] << " " << p[4] << " " << p[8] << " " << p[12] << " |\n";
    std::cout << "| " << p[1] << " " << p[5] << " " << p[9] << " " << p[13] << " |\n";
    std::cout << "| " << p[2] << " " << p[6] << " " << p[10] << " " << p[14] << " |\n";
    std::cout << "| " << p[3] << " " << p[7] << " " << p[11] << " " << p[15] << " |\n";
    std::cout << "---------------------------\n";
}

void runCameraTest() {
    std::cout << "=== INIZIO TEST CAMERA ===" << std::endl;

    // 1. Creazione Camera
    // FOV 60 gradi, Aspect 1.33 (800/600), Near 0.1, Far 100.0
    PerspectiveCamera cam("MainCam", 60.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    std::cout << "1. Camera creata: " << cam.getName() << std::endl;
    if (cam.getType() == "PerspectiveCamera") std::cout << "  [OK] Tipo corretto." << std::endl;
    else std::cout << "  [FAIL] Tipo errato: " << cam.getType() << std::endl;

    // 2. Verifica Matrice Iniziale
    glm::mat4 m1 = cam.getProjectionMatrix();
    printMatrix("Matrice Base", m1);

    // 3. Modifica Parametri (Zoom in)
    std::cout << "2. Modifico FOV (Zoom in a 30 gradi)..." << std::endl;
    cam.setFov(30.0f);

    glm::mat4 m2 = cam.getProjectionMatrix();
    printMatrix("Matrice Zoomata", m2);

    // Se le matrici sono diverse, vuol dire che il ricalcolo funziona
    if (m1 != m2) std::cout << "  [OK] La matrice è cambiata correttamente." << std::endl;
    else std::cout << "  [FAIL] La matrice NON è cambiata!" << std::endl;

    // 4. Test Node Inheritance
    // La camera è un nodo, quindi deve avere una matrice di trasformazione (posizione)
    std::cout << "3. Test ereditarietà Node..." << std::endl;
    glm::mat4 world = cam.getWorldMatrix();
    // Di base è l'identità
    if (world[3][3] == 1.0f) std::cout << "  [OK] Camera eredita correttamente da Node." << std::endl;

    std::cout << "=== FINE TEST CAMERA ===" << std::endl;
}

int main(int argc, char* argv[]) {
    runCameraTest();
    return 0;
}