#include "engine.h"

#ifdef _linux
#include <unistd.h>
#endif

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>

bool Engine::isInitializedFlag = false;
bool Engine::isRunningFlag = false;
int Engine::windowId = 0;

// scena caricata
std::shared_ptr<Node> Engine::scene;

std::shared_ptr<Camera> Engine::activeCamera;
int Engine::windowWidth = 0;
int Engine::windowHeight = 0;
std::string Engine::screenText;

// Materiale per le ombre
std::shared_ptr<Material> Engine::shadowMaterial = std::make_shared<Material>();

// Frames:
int Engine::frames = 0;
float Engine::fps = 0.0f;
void (*Engine::blinkingCallback)() = nullptr;

// Conta quanto tempo     passato
int timeCallback = 0;


// se il simbolo _WINDOWS     definito.
#ifdef _WINDOWS
#include <Windows.h>

// Questa funzione viene chiamata automaticamente dal sistema operativo quando un processo carica o scarica una DLL.

// HANDLE instDLL: Un handle alla DLL che viene caricato o scaricato.
// DWORD reason: Un codice che indica il motivo per cui la funzione DllMain     stata chiamata. 
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
    switch (reason)
    {
        // Un processo sta caricando la DLL. 
    case DLL_PROCESS_ATTACH:
        break;

        // Un processo sta scaricando la DLL.
    case DLL_PROCESS_DETACH:
        break;
    }

    return true;
}
#endif

/**
 * @brief Inizializza il motore grafico e crea una finestra con le specifiche fornite.
 *
 * Questa funzione configura e inizializza il motore grafico, inclusi i parametri della finestra,
 * il sistema di rendering OpenGL, e la libreria FreeImage per la manipolazione delle immagini.
 * Verifica anche se l'engine     gi    stato inizializzato per evitare inizializzazioni multiple.
 *
 * @param windowTitle Il titolo da assegnare alla finestra creata.
 * @param windowWidth La larghezza della finestra in pixel.
 * @param windowHeight L'altezza della finestra in pixel.
 *
 * @note Se l'engine     gi    stato inizializzato, questa funzione emette un errore e termina senza fare nulla.
 *
 * @warning Assicurati di chiamare questa funzione prima di qualsiasi altro metodo dell'engine.
 */
void LIB_API Engine::init(const std::string windowTitle, const int windowWidth, const int windowHeight)
{
    // Verifica se l'engine     gi    stato inizializzato
    if (Engine::isInitializedFlag)
    {
        ERROR("Engine has already been initialized.");
        return;
    }

    int argc = 0;

    // Inizializza la libreria FreeGLUT. 
    glutInit(&argc, NULL);

    // Abilita il doppio buffer e il depth buffer.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // Il ciclo principale di FreeLUT ritorna quando la finestra viene chiusa.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Crea una finestra con il titolo specificato e inizializza il contesto
    // !! Importante si pu    usare OpenGL solo dopo la sua invocazione
    Engine::windowId = glutCreateWindow(windowTitle.c_str());

    // Imposta la dimensione della finestra appena creata.
    glutReshapeWindow(windowWidth, windowHeight);

    // Come la superficie dei triangoli deve essere renderizzata.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Setup callbacks

    // Questa funzione verr    chiamata da FreeLUT ogni volta che     necessario ridisegnare la finestra.
    glutDisplayFunc(render);

    // Per la gestione del ridimensionamento della finestra.
    glutReshapeFunc(resizeCallback);

    // Imposta un callback timer che chiama timerCallback ogni 50 millisecondi 
    glutTimerFunc(50, timerCallback, 0);

    // Configura OpenGL
    // Abilita il test di profondit    per gestire la visibilit    degli oggetti.
    glEnable(GL_DEPTH_TEST);
    // Abilita la normalizzazione dei vettori per garantire che le normali siano sempre normalizzate.
    // Le normali devono essere nomalizzate se ci sono scaling nella GL_MODELVIEW matrix.
    glEnable(GL_NORMALIZE);
    // Abilita l'illuminazione per il rendering.
    glEnable(GL_LIGHTING);
    // Abilita il cull face.
    glEnable(GL_CULL_FACE);
    // Abilita il supporto per il mapping delle texture 2D.
    glEnable(GL_TEXTURE_2D);

    // Configura la global light -> colore grigio scuro
    // Verr    moltiplicato per il termine ambientale di ogni sorgente luminosa della scena
    const glm::vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));
    // Riflessi speculari pi    accurati.
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

    // Inizializza FreeImage
    FreeImage_Initialise();

    // Imposta il viewport iniziale per coprire l'intera finestra.
    glViewport(0, 0, windowWidth, windowHeight);

    // Configura il materiale per le ombre
    // Colore ambientale del materiale per le ombre a nero.
    Engine::shadowMaterial->setAmbientColor(glm::vec3(0.0f, 0.0f, 0.0f));
    // Colore diffuso del materiale per le ombre a nero.
    Engine::shadowMaterial->setDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));
    // Colore speculare del materiale per le ombre a nero.
    Engine::shadowMaterial->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));
    Engine::shadowMaterial->setShininess(0.0f);

    // Avvia l'engine
    Engine::isInitializedFlag = true;
    Engine::isRunningFlag = true;
}

///// Getter

/**
 * @brief Restituisce l'oggetto root della scena corrente.
 *
 * @return Un puntatore condiviso all'oggetto root della scena corrente.
 */
std::shared_ptr<Node> LIB_API Engine::getScene()
{
    return Engine::scene;
}

/**
 * @brief Restituisce il puntatore alla telecamera attiva.
 *
 * @return Un puntatore condiviso alla telecamera attiva.
 */
std::shared_ptr<Camera> LIB_API Engine::getActiveCamera() {
    return activeCamera;
}


///// Setter

/**
 * @brief Imposta il colore di sfondo della scena.
 *
 * @param red La quantit    di rosso nel colore di sfondo, compresa tra 0.0f e 1.0f.
 * @param green La quantit    di verde nel colore di sfondo, compresa tra 0.0f e 1.0f.
 * @param blue La quantit    di blu nel colore di sfondo, compresa tra 0.0f e 1.0f.
 */
void LIB_API Engine::setBackGround(const float red, const float green, const float blue)
{
    glClearColor(red, green, blue, 1.0f);
}

/**
 * @brief Imposta il testo da visualizzare sullo schermo.
 *
 * @param newText Il testo da visualizzare sullo schermo.
 */
void LIB_API Engine::setScreenText(const std::string newText)
{
    Engine::screenText = newText;
}

/**
 * @brief Imposta la scena da renderizzare.
 *
 * @param newScene Puntatore condiviso alla nuova scena da impostare.
 *
 * @note La camera attiva viene impostata su `nullptr`, il che significa che
 *       la nuova scena non avr    una camera attiva fino a quando non verr
 *       impostata tramite `setActiveCamera`.
 */
void LIB_API Engine::setScene(const std::shared_ptr<Node> newScene)
{
    Engine::scene = newScene;
    Engine::activeCamera = nullptr;
}

/**
 * @brief Imposta la camera attiva per il rendering della scena.
 *
 * @param newActiveCamera Puntatore condiviso alla nuova camera da impostare.
 */
void LIB_API Engine::setActiveCamera(const std::shared_ptr<Camera> newActiveCamera)
{
    if (Engine::activeCamera != nullptr)
    {
        // Disabilia la vecchia camera
        Engine::activeCamera->setActive(false);
    }

    // imposta le dimensioni della finestra della nuova camera
    newActiveCamera->setWindowSize(Engine::windowWidth, Engine::windowHeight);
    newActiveCamera->setActive(true);

    Engine::activeCamera = newActiveCamera;
}

///// Set Callback

/**
 * @brief Imposta un callback personalizzato per la gestione della pressione dei tasti.
 *
 * @param
 *      - newKeyboardCallback Puntatore alla funzione di callback che gestir    gli eventi di pressione dei tasti.
 *      - key     il codice del tasto premuto.
 *      - mouseX e mouseY sono le coordinate del mouse al momento della pressione (non usate).
 */
void LIB_API Engine::setKeyboardCallback(void (*newKeyboardCallback) (const unsigned char key, const int mouseX, const int mouseY))
{
    // Quale funzione deve essere chiamata ogni volta che l'utente preme un tasto sulla tastiera.
    glutKeyboardFunc(newKeyboardCallback);
}

/**
 * @brief Imposta un callback per gestire il lampeggiamento.
 *
 * @param callback Puntatore alla funzione di callback da chiamare per aggiornare
 * il lampeggiamento.
 */
void LIB_API Engine::setBlinkingCallback(void (*callback)()) {
    Engine::blinkingCallback = callback;
}

/**
 * @brief Imposta un callback per gestire gli eventi del mouse.
 *
 * @param fnct Puntatore alla funzione di callback per gestire gli eventi del mouse.
 *             La funzione callback deve accettare quattro parametri:
 *             - `int button`: Il pulsante del mouse che ha generato l'evento.
 *             - `int state`: Lo stato del pulsante del mouse (premuto o rilasciato).
 *             - `int mouseX`: La coordinata X del mouse al momento dell'evento.
 *             - `int mouseY`: La coordinata Y del mouse al momento dell'evento.
 */
void LIB_API Engine::setMouseCallback(void(*newMouseCallback)(int button, int state, int mouseX, int mouseY)) {
    glutMouseFunc(newMouseCallback);
}

/**
 * @brief Imposta una funzione di callback per gestire le pressioni di tasti speciali.
 *
 * Questa funzione registra un callback che viene chiamato ogni volta che un tasto speciale
 * (come le frecce direzionali) viene premuto.
 *
 * @param
         - newSpecialCallback Puntatore alla funzione di callback.
 *       - key codice del tasto speciale premuto.
 *       - mouseX e mouseY sono le coordinate del mouse al momento della pressione.
 */
void LIB_API Engine::setMethodSpecialCallback(void(*newSpecialCallback) (int key, int mouseX, int mouseY)) {
    glutSpecialFunc(newSpecialCallback);
}

/**
 * @brief Renderizza la scena corrente.
 *
 * Questa funzione gestisce il rendering dell'intera scena, inclusi gli oggetti 3D, le ombre,
 * e il testo visualizzato sullo schermo.
 *
 * Se la scena o la telecamera non sono impostate, la funzione non esegue alcun rendering.
 */
void LIB_API Engine::render()
{
    // Se non ce la scena o la telecamera esce
    if (Engine::scene == nullptr || Engine::activeCamera == nullptr)
        return;

    Engine::activeCamera->setWindowSize(Engine::windowWidth, Engine::windowHeight);

    int maxNrOfLights;
    // Ottiene il numero massimo di luci che OpenGL pu    gestire.
    glGetIntegerv(GL_MAX_LIGHTS, &maxNrOfLights);

    // Disabilita tutte le luci.
    for (int i = 0; i < maxNrOfLights; i++)
        glDisable(GL_LIGHT0 + i);

    // metodo ricorsivo --> Analizza tutti i nodi figli del nodo che lo invoca
    // invocare pass sul root --> aggiunge il contenuto del grafo alla lista
    std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> render = List::pass(Engine::scene, glm::mat4(1.0f));

    List renderList;
    renderList.setListRendering(render);

    // Riordina la lista
    renderList.sortListRendering();

    // Ottiene l'inversa della camera matrix
    const glm::mat4 inverseCameraMatrix = Engine::activeCamera->getInverseMatrix();

    // Renderizza tutta la lista
    renderList.render(inverseCameraMatrix);

    // Fake shadow -> Rendering delle ombre

    // Modifica la funzione di confronto del buffer di profondit    per permettere la scrittura anche per i pixel con la stessa profondit   .
    glDepthFunc(GL_LEQUAL);

    // Crea una matrice per ridurre l'altezza delle ombre.
    const glm::mat4 shadowModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.0f));

    for (const auto& node : renderList.getListRendering())
    {
        std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(node.first);

        // se     un Mesh e pu    proiettare ombre  
        if (mesh != nullptr && mesh->getShadows())
        {
            // salva il materiale originale
            const std::shared_ptr<Material> originalMaterial = mesh->getMaterial();

            // Cambia temporaneamente il materiale della mesh con quello dell'ombra.
            mesh->setMaterial(Engine::shadowMaterial);
            const glm::mat4 shadow_matrix = shadowModelScaleMatrix * node.second;

            // Si renderizza l'ombra.
            mesh->render(inverseCameraMatrix * shadow_matrix);

            // Si rimette il materiale originale della mesh.
            mesh->setMaterial(originalMaterial);
        }
    }

    // Ripristina la funzione di confronto del buffer di profondit    originale.
    glDepthFunc(GL_LESS);

    // Pulisce il buffer di profondit   , assicurando che il testo renderizzato appaia sopra la scena 3D.
    glClear(GL_DEPTH_BUFFER_BIT); // Cos    la victory screen appare avanti


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, (float)Engine::windowWidth, 0.0f, (float)Engine::windowHeight, -1.0f, 1.0f)));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

    // Disabilita l'illuminazione per il rendering del testo
    glDisable(GL_LIGHTING);

    // Imposta il colore del testo su bianco.
    glColor3f(1.0f, 1.0f, 1.0f);

    // Imposta la posizione del testo da renderizzare.
    glRasterPos2f(16.0f, 5.0f);

    std::string fps = "FPS: " + std::to_string((int)Engine::fps);

    // Disegna il testo "FPS" e il testo della schermata.
    glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)fps.c_str());

    // Imposta il punto di partenza per il rendering del testo
    glRasterPos2f(16.0f, Engine::windowHeight - 32.0f);

    // Disegna il testo specificato nella posizione impostata.
    glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)Engine::screenText.c_str());

    // Riattiva l'illuminazione.
    glEnable(GL_LIGHTING);

    // Incrementa il conteggio dei frame: 
    Engine::frames++;
}

/**
 * @brief Callback del tempo (timer)
 *
 * Questa funzione viene chiamata periodicamente tramite `glutTimerFunc` per aggiornare
 * il conteggio dei frames e calcolare il tempo trascorso per il blinking.
 *
 * @param value Il parametro utilizzato da `glutTimerFunc()`. Non     utilizzato nella funzione.
 */
void LIB_API Engine::timerCallback(int value)
{
    timeCallback++; // ogni 50 ms

    // Ogni secondo aggiorna il framerate
    if (timeCallback == 20) {
        Engine::fps = Engine::frames;
        Engine::frames = 0;
        std::cout << "fps: " << Engine::fps << std::endl;
        timeCallback = 0;
    }

    // Esegui il callback per il lampeggiamento se definito
    if (Engine::blinkingCallback != nullptr) {
        Engine::blinkingCallback();
    }

    // Richiamata dopo 50 ms
    glutTimerFunc(50, timerCallback, 0);
}

/**
 * @brief Vengono eseguite le callback.
 *
 * Questa funzione chiama `glutMainLoopEvent` per elaborare eventi come input da tastiera, mouse
 * o eventi di ridimensionamento della finestra. Serve a garantire che tutte le callback registrate
 * per questi eventi siano eseguite.
 *
 */
void LIB_API Engine::update()
{
    // Chiamandola vengono gestite le callback
    glutMainLoopEvent();
}

/**
 * @brief Pulisce il contenuto della finestra di rendering.
 *
 * @details
 * - `GL_COLOR_BUFFER_BIT` specifica che il buffer del colore deve essere azzerato.
 * - `GL_DEPTH_BUFFER_BIT` specifica che il buffer di profondit    deve essere azzerato.
 */
void LIB_API Engine::clearScreen()
{
    // Preparare la finestra per un nuovo frame di rendering.
    // Se no i valori del z buffer non vengono aggiornarti.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Scambia i buffer back e front.
 *
 * Questa funzione utilizza `glutSwapBuffers` per scambiare il buffer anteriore con quello posteriore
 * in un'applicazione che utilizza il doppio buffering. Il buffer front contiene il frame attualmente
 * visualizzato, mentre il buffer back contiene il prossimo frame da visualizzare.
 */
void LIB_API Engine::swapBuffers()
{
    glutSwapBuffers();
}

/**
 * @brief Invia una direttiva di arresto all'engine.
 */
void LIB_API Engine::stop()
{
    Engine::isRunningFlag = false;
}

/**
 * @brief De-inizializza l'engine.
 *
 * Questa funzione libera le risorse utilizzate dall'engine e chiude il ciclo principale di GLUT.
 * Dovrebbe essere chiamata solo una volta alla fine dell'uso dell'engine e nessun'altra funzione dell'engine
 * dovrebbe essere chiamata dopo questa.
 */
void LIB_API Engine::quit()
{
    // Utilizzata per liberare le risorse e fare la pulizia finale quando si termina l'uso della libreria FreeImage.
    FreeImage_DeInitialise();

    // Uscire dal ciclo principale di GLUT
    glutLeaveMainLoop();
}

/**
 * @brief Restituisce lo stato del flag di esecuzione.
 *
 * @return `true` se l'engine     ancora in esecuzione, `false` se     stato fermato.
 */
bool LIB_API Engine::isRunning()
{
    return Engine::isRunningFlag;
}

/**
 * @brief Gestisce il ridimensionamento della finestra.
 *
 * Questa funzione viene chiamata automaticamente da GLUT quando la finestra viene ridimensionata.
 *
 * @param width La nuova larghezza della finestra.
 * @param height La nuova altezza della finestra.
 */
void LIB_API Engine::resizeCallback(const int width, const int height)
{
    // Larghezza attuale della finestra dopo il ridimensionamento.
    Engine::windowWidth = width;
    // Altezza attuale della finestra dopo il ridimensionamento.
    Engine::windowHeight = height;

    std::cout << "width: " << Engine::windowWidth << std::endl;
    std::cout << "height: " << Engine::windowHeight << std::endl;

    // Se esiste una camera attiva 
    if (Engine::activeCamera != nullptr)
        Engine::activeCamera->setWindowSize(Engine::windowWidth, Engine::windowHeight);

    // Viewport: definisce l'area della finestra in cui disegnare.
    // Copre l'intera finestra, partendo dall'angolo in basso a sinistra (0,0) fino alle nuove dimensioni.
    glViewport(0, 0, width, height);
}

/**
 * @brief Trova un oggetto nella scena corrente per nome.
 *
 * @param nameToFind Il nome dell'oggetto da trovare.
 * @return Un puntatore condiviso all'oggetto trovato, oppure nullptr se nessun oggetto con il nome specificato     stato trovato.
 */
std::shared_ptr<Node> LIB_API Engine::findObjectByName(const std::string nameToFind)
{
    const auto object = findObjectByName(nameToFind, Engine::scene);

    // Se l'oggetto con il nome specificato non     stato trovato.
    if (object == nullptr)
        WARNING("Could not find object with name \"" << nameToFind << "\".");

    return object;
}

/**
 * @brief Trova un oggetto nella scena corrente per nome.
 *
 * Questa funzione cerca ricorsivamente un oggetto nella scena corrente a partire dal nodo radice specificato.
 *
 * @param nameToFind Il nome dell'oggetto da trovare.
 * @param root Il nodo radice da cui iniziare la ricerca.
 * @return Un puntatore condiviso all'oggetto trovato, oppure nullptr se nessun oggetto con il nome specificato     stato trovato.
 */
std::shared_ptr<Node> LIB_API Engine::findObjectByName(const std::string nameToFind, const std::shared_ptr<Node> root)
{
    // Itera su tutti i figli del nodo root.
    for (const auto& child : root->getChildren())
    {
        // Verifica se il nome del nodo figlio corrisponde al nome cercato.
        if (child->getName() == nameToFind)
            return child;

        // Continua la ricerca ricorsiva nei figli del nodo corrente.
        const auto found = Engine::findObjectByName(nameToFind, child);

        if (found != nullptr)
            return found;
    }

    return nullptr;
}

/**
 * @brief Recupera un nodo selezionato cliccando con il mouse.
 *
 * Questa funzione esegue il rendering della scena in modalit    color picking per determinare quale
 * oggetto     stato selezionato con un clic del mouse. Utilizza la lettura del colore del pixel sotto
 * il cursore per identificare l'oggetto.
 *
 * @param mouseX La coordinata X del clic del mouse nella finestra.
 * @param mouseY La coordinata Y del clic del mouse nella finestra.
 *
 * @return Un puntatore condiviso al nodo selezionato, oppure `nullptr` se nessun nodo corrisponde
 *         al clic del mouse.
 *
 * @note La modalit    di color picking viene abilitata durante il rendering per associare un colore
 *       unico a ciascun oggetto. Dopo il rendering, viene letto il colore del pixel sotto il cursore
 *       per determinare l'ID dell'oggetto. La modalit    di color picking viene disabilitata una volta
 *       completata l'operazione.
 */
std::shared_ptr<Node> LIB_API Engine::getNodeByClick(int mouseX, int mouseY)
{
    // Abilita il color picking
    Mesh::isColorPickingMode = true;

    // Esegui il rendering in modalit    color picking
    Engine::render();

    // Legge il colore del pixel sotto il cursore
    unsigned char pixel[4];
    glReadPixels(mouseX, Engine::windowHeight - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    // Disabilita il color picking
    Mesh::isColorPickingMode = false;

    // Usa l'id per trovare l'oggetto selezionato.
    int id = pixel[0];
    std::shared_ptr<Node> selectedObject = Engine::findObjectByID(id);

    return selectedObject;
}

/**
 * @brief Trova un oggetto nella scena corrente per ID.
 *
 * @param idToFind L'ID dell'oggetto da trovare.
 * @return Un puntatore condiviso all'oggetto trovato, oppure nullptr se nessun oggetto con l'ID specificato     stato trovato.
 */
std::shared_ptr<Node> LIB_API Engine::findObjectByID(int idToFind)
{
    const auto object = findObjectByID(idToFind, Engine::scene);

    // Se l'oggetto con l'ID specificato non     stato trovato.
    if (object == nullptr)
        WARNING("Could not find object with ID \"" << idToFind << "\".");

    return object;
}

/**
 * @brief Trova un oggetto nella scena corrente per ID.
 *
 * Questa funzione cerca ricorsivamente un oggetto nella scena corrente a partire dal nodo radice specificato.
 *
 * @param idToFind L'ID dell'oggetto da trovare.
 * @param root Il nodo radice da cui iniziare la ricerca.
 * @return Un puntatore condiviso all'oggetto trovato, oppure nullptr se nessun oggetto con l'ID specificato     stato trovato.
 */
std::shared_ptr<Node> LIB_API Engine::findObjectByID(int idToFind, const std::shared_ptr<Node> root)
{
    // Verifica se il nodo corrente ha l'ID che cerchiamo.
    if (root->getId() == idToFind)
        return root;

    // Itera su tutti i figli del nodo root.
    for (const auto& child : root->getChildren())
    {
        // Continua la ricerca ricorsiva nei figli del nodo corrente.
        const auto found = Engine::findObjectByID(idToFind, child);

        if (found != nullptr)
            return found;
    }

    return nullptr;
}

bool LIB_API Engine::removeObject(const std::shared_ptr<Node>& nodeToRemove)
{
    if (!scene)
    {
        std::cerr << "Error: Scene is not set. Cannot remove object." << std::endl;
        return false;
    }

    // Chiama la versione con due parametri, usando `scene` come radice
    return removeObject(nodeToRemove, scene);
}

bool LIB_API Engine::removeObject(const std::shared_ptr<Node>& nodeToRemove, const std::shared_ptr<Node>& root)
{

    auto& children = root->getChildren(); // Ottieni i figli del nodo corrente

    // Itera sui figli del nodo radice
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (*it == nodeToRemove)
        {
            // Nodo trovato, rimuovilo dal vettore dei figli
            children.erase(it);
            std::cout << "Removed node: " << nodeToRemove->getName() << std::endl;
            return true;
        }

        // Ricorsione per cercare nei figli
        if (removeObject(nodeToRemove, *it))
        {
            return true;
        }
    }

    return false; // Nodo non trovato
}

/**
 * @brief Rimuove tutti i nodi figli dalla scena corrente.
 */
void LIB_API Engine::removeAllObjects() {
    if (!scene) {
        std::cerr << "Error: Scene is not set. Cannot remove objects." << std::endl;
        return;
    }

    // Rimuove tutti i figli della scena
    scene->removeAllChildren();
    std::cout << "All objects have been removed from the scene." << std::endl;
}



/**
 * @brief Restituisce la matrice di trasformazione globale di un nodo.
 *
 * Calcola ricorsivamente la trasformazione globale combinata di un nodo
 * sommando le trasformazioni di tutti i suoi genitori nella gerarchia della scena.
 *
 * @param node Il nodo di cui calcolare la matrice globale.
 * @return La matrice di trasformazione globale.
 */
glm::mat4 Engine::getGlobalTransform(const std::shared_ptr<Node>& node) {
    if (!node) {
        std::cerr << "Error: Node is null. Cannot compute global transform." << std::endl;
        return glm::mat4(1.0f);
    }
    if (!node->getParent()) {
        return node->getTransform();
    }
    return getGlobalTransform(node->getParent()) * node->getTransform();
}

glm::vec3 Engine::getGlobalPosition(const std::shared_ptr<Node>& node) {
    glm::mat4 globalTransform = getGlobalTransform(node);
    return glm::vec3(globalTransform[3][0], globalTransform[3][1], globalTransform[3][2]);
}

