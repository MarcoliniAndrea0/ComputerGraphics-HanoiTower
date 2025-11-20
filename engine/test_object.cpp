#include "object.h"
#include <iostream>
#include <vector>

/**
 * @class TestObject
 * @brief Classe concreta derivata da Object per scopi di testing.
 *
 * Implementa il metodo virtuale puro render() per permettere
 * l'istanziazione e il test delle funzionalità della classe base.
 */
class TestObject : public Object
{
public:
    TestObject(const std::string& name = "", const std::string& type = "")
        : Object(name, type) {
    }

    /**
     * @brief Implementazione concreta del metodo render.
     * Stampa informazioni sull'oggetto.
     */
    void render() override
    {
        std::cout << "Rendering Object [ID: " << id
            << ", Name: '" << name
            << "', Type: '" << type << "']" << std::endl;
    }
};

/**
 * @brief Funzione main per testare la classe Object.
 *
 * Esegue una serie di test per verificare:
 * - Creazione di oggetti con parametri di default
 * - Creazione di oggetti con nome e tipo personalizzati
 * - Assegnazione automatica di ID univoci
 * - Funzionamento dei getter e setter
 * - Chiamata al metodo render
 */
int main()
{
    std::cout << "=== TEST CLASSE OBJECT ===" << std::endl << std::endl;

    // Test 1: Costruzione con parametri di default
    std::cout << "Test 1: Oggetto con parametri di default" << std::endl;
    TestObject obj1;
    std::cout << "ID: " << obj1.getID() << std::endl;
    std::cout << "Nome: '" << obj1.getName() << "'" << std::endl;
    std::cout << "Tipo: '" << obj1.getType() << "'" << std::endl;
    obj1.render();
    std::cout << std::endl;

    // Test 2: Costruzione con nome e tipo
    std::cout << "Test 2: Oggetto con nome e tipo" << std::endl;
    TestObject obj2("Cubo", "Mesh");
    std::cout << "ID: " << obj2.getID() << std::endl;
    std::cout << "Nome: '" << obj2.getName() << "'" << std::endl;
    std::cout << "Tipo: '" << obj2.getType() << "'" << std::endl;
    obj2.render();
    std::cout << std::endl;

    // Test 3: Verifica unicità degli ID
    std::cout << "Test 3: Creazione multipla e verifica ID univoci" << std::endl;
    TestObject obj3("Sfera", "Geometria");
    TestObject obj4("Luce", "Light");
    TestObject obj5("Camera", "Camera");

    std::cout << "obj3 ID: " << obj3.getID() << " - Nome: " << obj3.getName() << std::endl;
    std::cout << "obj4 ID: " << obj4.getID() << " - Nome: " << obj4.getName() << std::endl;
    std::cout << "obj5 ID: " << obj5.getID() << " - Nome: " << obj5.getName() << std::endl;
    std::cout << std::endl;

    // Test 4: Test dei setter
    std::cout << "Test 4: Modifica tramite setter" << std::endl;
    TestObject obj6("VecchioNome", "VecchioTipo");
    std::cout << "Prima della modifica:" << std::endl;
    obj6.render();

    obj6.setName("NuovoNome");
    obj6.setType("NuovoTipo");
    std::cout << "Dopo la modifica:" << std::endl;
    obj6.render();
    std::cout << std::endl;

    // Test 5: Array di oggetti tramite puntatori alla classe base
    std::cout << "Test 5: Array di puntatori a Object (polimorfismo)" << std::endl;
    std::vector<Object*> sceneGraph;

    sceneGraph.push_back(new TestObject("Oggetto1", "Mesh"));
    sceneGraph.push_back(new TestObject("Oggetto2", "Light"));
    sceneGraph.push_back(new TestObject("Oggetto3", "Camera"));

    std::cout << "Rendering di tutti gli oggetti nella scena:" << std::endl;
    for (Object* obj : sceneGraph)
    {
        obj->render();
    }

    // Pulizia memoria
    for (Object* obj : sceneGraph)
    {
        delete obj;
    }
    sceneGraph.clear();
    std::cout << std::endl;

    // Test 6: Verifica incremento continuo ID
    std::cout << "Test 6: Verifica continuità contatore ID" << std::endl;
    TestObject obj7("Test7", "Type7");
    TestObject obj8("Test8", "Type8");
    std::cout << "L'ID dovrebbe continuare ad incrementare: " << std::endl;
    std::cout << "obj7 ID: " << obj7.getID() << std::endl;
    std::cout << "obj8 ID: " << obj8.getID() << std::endl;
    std::cout << std::endl;

    std::cout << "=== TUTTI I TEST COMPLETATI ===" << std::endl;

    return 0;
}
