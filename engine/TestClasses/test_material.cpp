/**
 * @file test_material.cpp
 * @brief Test unitario per la classe Material.
 */
#include <iostream>
#include <string>
#include <cmath> // per std::abs
#include "../material.h"
#include "../engine.h" 

 // Funzione helper per confrontare due float con un margine di errore
bool isClose(float a, float b, float epsilon = 0.001f) {
    return std::abs(a - b) < epsilon;
}

// Funzione helper per verificare i vettori colore
bool checkColor(const std::string& label, const glm::vec4& actual, const glm::vec4& expected) {
    bool ok = isClose(actual.r, expected.r) &&
        isClose(actual.g, expected.g) &&
        isClose(actual.b, expected.b) &&
        isClose(actual.a, expected.a);

    if (ok) {
        std::cout << "  [OK] " << label << std::endl;
    }
    else {
        std::cout << "  [FAIL] " << label
            << " Atteso: (" << expected.r << "," << expected.g << "," << expected.b << ")"
            << " Trovato: (" << actual.r << "," << actual.g << "," << actual.b << ")" << std::endl;
    }
    return ok;
}

void runMaterialTest() {
    std::cout << "=== INIZIO TEST MATERIAL ===" << std::endl;

    // ---------------------------------------------------
    // 1. Test Costruttore e Default
    // ---------------------------------------------------
    std::cout << "\n1. Test Valori di Default:" << std::endl;
    Material mat("MatTest");

    // Verifica nome
    if (mat.getName() == "MatTest") std::cout << "  [OK] Nome impostato correttamente." << std::endl;
    else std::cout << "  [FAIL] Nome errato." << std::endl;

    // Verifica un valore di default (Ambient dovrebbe essere 0.1, 0.1, 0.1, 1.0)
    checkColor("Ambient Default", mat.getAmbient(), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));


    // ---------------------------------------------------
    // 2. Test Setters (Modifica valori)
    // ---------------------------------------------------
    std::cout << "\n2. Test Setters:" << std::endl;

    glm::vec4 rosso(1.0f, 0.0f, 0.0f, 1.0f);
    mat.setDiffuse(rosso);
    checkColor("Set Diffuse (Rosso)", mat.getDiffuse(), rosso);

    glm::vec4 bianco(1.0f, 1.0f, 1.0f, 1.0f);
    mat.setSpecular(bianco);
    checkColor("Set Specular (Bianco)", mat.getSpecular(), bianco);


    // ---------------------------------------------------
    // 3. Test Logica (Clamping Shininess)
    // ---------------------------------------------------
    std::cout << "\n3. Test Clamping Shininess (0-128):" << std::endl;

    // Caso A: Valore valido
    mat.setShininess(50.0f);
    if (isClose(mat.getShininess(), 50.0f)) std::cout << "  [OK] Valore valido (50) accettato." << std::endl;
    else std::cout << "  [FAIL] Valore valido rifiutato." << std::endl;

    // Caso B: Overflow (> 128) -> Deve diventare 128
    mat.setShininess(200.0f);
    if (isClose(mat.getShininess(), 128.0f)) std::cout << "  [OK] Overflow corretto a 128." << std::endl;
    else std::cout << "  [FAIL] Overflow non gestito! Valore: " << mat.getShininess() << std::endl;

    // Caso C: Underflow (< 0) -> Deve diventare 0
    mat.setShininess(-10.0f);
    if (isClose(mat.getShininess(), 0.0f)) std::cout << "  [OK] Underflow corretto a 0." << std::endl;
    else std::cout << "  [FAIL] Underflow non gestito! Valore: " << mat.getShininess() << std::endl;


    // ---------------------------------------------------
    // 4. Test Chiamata Render (Smoke Test)
    // ---------------------------------------------------
    std::cout << "\n4. Smoke Test Render:" << std::endl;
    // Proviamo a chiamare render(). Anche se non c'è una finestra, non deve crashare.
    // (Nota: su alcuni driver potrebbe dare warning, ma verifichiamo che il codice corra)
    try {
        mat.render();
        std::cout << "  [OK] mat.render() eseguito senza crash." << std::endl;
    }
    catch (...) {
        std::cout << "  [FAIL] mat.render() ha generato un'eccezione." << std::endl;
    }

    std::cout << "\n=== FINE TEST MATERIAL ===" << std::endl;
}

// MAIN
int main(int argc, char* argv[]) {
    runMaterialTest();
    return 0;
}