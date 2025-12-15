#include "hanoi.h"
#include <iostream>
#include <algorithm> // per std::find

// Costanti
const int HanoiGame::NUM_DISKS = 7;
const int HanoiGame::NUM_TOWERS = 3;
const float HanoiGame::DISK_HEIGHT = 25.0f;
const float HanoiGame::TOWER_DISTANCE = 300.0f;
const float HanoiGame::DISK_SPACING = 5.0f;

// Variabili statiche
HanoiGame::GameState HanoiGame::gameState = GameState::IDLE;
std::vector<HanoiGame::Disk> HanoiGame::disks;
std::vector<HanoiGame::Tower> HanoiGame::towers;
int HanoiGame::selectedDiskIndex = -1;
int HanoiGame::moveCount = 0;
int HanoiGame::minMovesRequired = 0;
float HanoiGame::animationTime = 0.0f;
std::shared_ptr<Node> HanoiGame::sceneRoot = nullptr;
std::stack<HanoiGame::MoveAction> HanoiGame::undoStack;
std::stack<HanoiGame::MoveAction> HanoiGame::redoStack;

// Helper ricorsivo per trovare un nodo
std::shared_ptr<Node> HanoiGame::findNode(std::shared_ptr<Node> root, const std::string& name) {
    if (!root) return nullptr;
    if (root->getName() == name) return root;

    for (auto& child : root->getChildren()) {
        auto result = findNode(child, name);
        if (result) return result;
    }
    return nullptr;
}

void HanoiGame::init(std::shared_ptr<Node> rootNode) {
    std::cout << "Inizializzazione gioco Torre di Hanoi..." << std::endl;

    sceneRoot = rootNode; // Salviamo il root per il reset
    gameState = GameState::IDLE;
    selectedDiskIndex = -1;
    moveCount = 0;
    minMovesRequired = (1 << NUM_DISKS) - 1;

    disks.clear();
    towers.clear();

    loadGameObjects(rootNode);
    updateDiskPositions(); // Posiziona visivamente i dischi
}

void HanoiGame::loadGameObjects(std::shared_ptr<Node> rootNode) {
    // --- 1. TROVIAMO L'ANCORA (IL DISCO 1) ---
    // Usiamo il disco più grande come punto di riferimento per tutto il gioco
    glm::vec3 anchorPos = glm::vec3(0.0f);
    bool foundAnchor = false;

    // Cerchiamo Disk_1
    auto anchorDisk = findNode(rootNode, "Disk_1");
    if (anchorDisk) {
        anchorPos = anchorDisk->getPosition();
        foundAnchor = true;
        std::cout << "DEBUG: ANCORA TROVATA (Disk_1) a: "
            << anchorPos.x << ", " << anchorPos.y << ", " << anchorPos.z << std::endl;
    }
    else {
        std::cerr << "ERRORE CRITICO: Disk_1 non trovato! Uso (0,0,0)" << std::endl;
    }

    // --- 2. CREIAMO LE TORRI ---

    for (int i = 1; i <= NUM_TOWERS; i++) {
        Tower tower;
        tower.name = "Tower_" + std::to_string(i);
        tower.node = findNode(rootNode, tower.name);

        if (tower.node) {
            // Calcoliamo la posizione LOGICA dove andranno i dischi
            glm::vec3 logicPos = anchorPos;

            logicPos.z += (i - 1) * TOWER_DISTANCE;

            tower.position = logicPos;

            std::cout << "Logica " << tower.name << " impostata a " << logicPos.z << std::endl;
            tower.diskIndices.clear();
        }
        towers.push_back(tower);
    }

    // --- 3. CARICHIAMO I DISCHI ---
    for (int i = 1; i <= NUM_DISKS; i++) {
        Disk disk;
        disk.size = i;
        disk.name = "Disk_" + std::to_string(i);
        disk.currentTower = 0;

        disk.node = findNode(rootNode, disk.name);

        if (!disk.node) std::cerr << "ERRORE: " << disk.name << " mancante" << std::endl;

        disks.push_back(disk);
        towers[0].diskIndices.push_back(i - 1);
    }

    // Allineiamo tutto
    updateDiskPositions();
}

void HanoiGame::reset() {
    if (sceneRoot) {
        init(sceneRoot);
        Engine::setScreenText("Resetting Tower of Hanoi game...");
    }
}

/*
void HanoiGame::handleClick(int mouseX, int mouseY) {
    if (gameState == GameState::GAME_WON) return;

    auto clickedNode = Engine::getNodeByClick(mouseX, mouseY);
    if (!clickedNode) return;

    std::string name = clickedNode->getName();

    // --- FASE 1: SELEZIONE DEL DISCO ---
    if (gameState == GameState::IDLE) {
        // Cerca se abbiamo cliccato un disco
        int diskIdx = findDiskIndex(name);

        // Se abbiamo cliccato una torre, vediamo se ha dischi e selezioniamo l'ultimo
        if (diskIdx == -1) {
            int towerIdx = findTowerIndex(name);
            if (towerIdx != -1 && !towers[towerIdx].diskIndices.empty()) {
                diskIdx = towers[towerIdx].diskIndices.back();
            }
        }

        if (diskIdx != -1) {
            // Controlla se il disco è in cima alla sua torre (regola fondamentale)
            int currentTowerIdx = disks[diskIdx].currentTower;
            if (towers[currentTowerIdx].diskIndices.back() == diskIdx) {
                selectedDiskIndex = diskIdx;
                gameState = GameState::DISK_SELECTED;
                std::cout << "Selezionato: " << disks[diskIdx].name << std::endl;
                updateDiskPositions(); // Per applicare l'effetto "sollevamento"
            }
            else {
                std::cout << "Puoi muovere solo il disco in cima alla pila!" << std::endl;
            }
        }
    }
    // --- FASE 2: SPOSTAMENTO ---
    else if (gameState == GameState::DISK_SELECTED) {
        // Abbiamo un disco in mano, dove lo mettiamo?
        int targetTowerIdx = findTowerIndex(name);

        // Se clicchiamo un disco, troviamo a quale torre appartiene
        if (targetTowerIdx == -1) {
            int clickedDiskIdx = findDiskIndex(name);
            if (clickedDiskIdx != -1) {
                targetTowerIdx = disks[clickedDiskIdx].currentTower;
            }
        }

        // Se abbiamo trovato una torre valida
        if (targetTowerIdx != -1) {
            if (performMove(selectedDiskIndex, targetTowerIdx)) {
                // Mossa riuscita
                selectedDiskIndex = -1;
                gameState = GameState::IDLE;
            }
            else {
                // Mossa invalida, deseleziona
                std::cout << "Mossa Annullata." << std::endl;
                selectedDiskIndex = -1;
                gameState = GameState::IDLE;
                updateDiskPositions();
            }
        }
        else {
            // Cliccato nel vuoto o oggetto invalido, deseleziona
            selectedDiskIndex = -1;
            gameState = GameState::IDLE;
            updateDiskPositions();
        }
    }
}
*/
/*
void HanoiGame::handleClick(int mouseX, int mouseY) {
    if (gameState == GameState::GAME_WON) return;

    auto clickedNode = Engine::getNodeByClick(mouseX, mouseY);
    if (!clickedNode) return;

    std::string name = clickedNode->getName();

    // Troviamo quale torre è coinvolta nel click
    int towerIdx = findTowerIndex(name);

    // Se non abbiamo cliccato direttamente una torre, controlliamo se è un disco
    if (towerIdx == -1) {
        int clickedDiskIdx = findDiskIndex(name);
        if (clickedDiskIdx != -1) {
            // Se clicco un disco, ottengo la torre su cui si trova
            towerIdx = disks[clickedDiskIdx].currentTower;
        }
    }

    // Se abbiamo identificato una torre valida, usiamo la logica unificata
    if (towerIdx != -1) {
        processTowerInput(towerIdx);
    }
    else {
        // Cliccato nel nulla -> Deseleziona se necessario
        if (gameState == GameState::DISK_SELECTED) {
            selectedDiskIndex = -1;
            gameState = GameState::IDLE;
            updateDiskPositions();
        }
    }
}
*/
void HanoiGame::processTowerInput(int towerIdx) {
    if (towerIdx < 0 || towerIdx >= NUM_TOWERS) return;

    // FASE 1: Selezionare un disco dalla torre indicata
    if (gameState == GameState::IDLE) {
        if (towers[towerIdx].diskIndices.empty()) {
            std::cout << "La torre " << (towerIdx + 1) << " e' vuota!" << std::endl;
            return;
        }

        // Seleziona il disco in cima
        int diskIdx = towers[towerIdx].diskIndices.back();
        selectedDiskIndex = diskIdx;
        gameState = GameState::DISK_SELECTED;

        std::cout << "Selezionato: " << disks[diskIdx].name << " dalla Torre " << (towerIdx + 1) << std::endl;
        updateDiskPositions(); // Solleva il disco
    }
    // FASE 2: Spostare il disco selezionato nella torre indicata
    else if (gameState == GameState::DISK_SELECTED) {
        // Se proviamo a rimetterlo nella stessa torre, deselezioniamo (annulla)
        if (disks[selectedDiskIndex].currentTower == towerIdx) {
            std::cout << "Deselezionato." << std::endl;
            selectedDiskIndex = -1;
            gameState = GameState::IDLE;
            updateDiskPositions(); // Riabbassa il disco
            return;
        }

        // Tentativo di movimento
        if (performMove(selectedDiskIndex, towerIdx)) {
            // Mossa OK
            selectedDiskIndex = -1;
            gameState = GameState::IDLE;
        }
        else {
            // Mossa non valida
            std::cout << "Mossa non valida!" << std::endl;
            // Rimane selezionato per provare un'altra torre, oppure puoi deselezionare qui
        }
    }
}

void HanoiGame::handleKey(unsigned char key) {
    switch (key) {
        case 'r':
        case 'R':
            reset();
            break;
        case 'h':
        case 'H':
            showHelp();
            break;
        case '3':
            processTowerInput(0); // Torre 1 (indice 0)
            break;
        case '2':
            processTowerInput(1); // Torre 2 (indice 1)
            break;
        case '1':
            processTowerInput(2); // Torre 3 (indice 2)
            break;
    }
}

std::string HanoiGame::getStatus() {
    if (gameState == GameState::GAME_WON) {
        return "YOU WIN! Moves: " + std::to_string(moveCount) + 
               "/" + std::to_string(minMovesRequired);
    }
    return "Tower of Hanoi - Moves: " + std::to_string(moveCount) + 
           "/" + std::to_string(minMovesRequired);
}

void HanoiGame::update(/*float deltaTime*/) {
    /*
    if (animationTime > 0) {
        animationTime -= deltaTime;
        // Update animation state here if needed
    }
    */
}

bool HanoiGame::isGameWon() {
    return gameState == GameState::GAME_WON;
}

int HanoiGame::getMoveCount() {
    return moveCount;
}

int HanoiGame::getMinMoves() {
    return minMovesRequired;
}

void HanoiGame::showHelp() {
    std::cout << "=== Tower of Hanoi Instructions ===" << std::endl;
    std::cout << "Objective: Move all disks to the rightmost tower" << std::endl;
    std::cout << "Rules:" << std::endl;
    std::cout << "1. Only one disk can be moved at a time" << std::endl;
    std::cout << "2. Only the top disk of a tower can be moved" << std::endl;
    std::cout << "3. A larger disk cannot be placed on a smaller disk" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- Click a disk to select it" << std::endl;
    std::cout << "- Click a tower to move selected disk there" << std::endl;
    std::cout << "- R: Reset game" << std::endl;
    std::cout << "- P: Show this help" << std::endl;
    std::cout << "- ESC: Exit game" << std::endl;
    std::cout << "Minimum moves required: " << minMovesRequired << std::endl;
    std::cout << "=================================" << std::endl;
    
    Engine::setScreenText("Goal: Move all 7 disks to right tower. Min moves: " + 
                         std::to_string(minMovesRequired));
}

// Private helper methods

void HanoiGame::updateDiskPositions() {
    for (int towerIdx = 0; towerIdx < NUM_TOWERS; towerIdx++) {
        auto& tower = towers[towerIdx];

        // Altezza di partenza: la posizione Y della torre + un piccolo offset se necessario
        
        float currentY = tower.position.y;
        std::cout << "TOWER POSITION : " << currentY << std::endl;

        
        //float currentY = 8750.0f;

        int index = tower.diskIndices.size();
        // Iteriamo sui dischi di questa torre
        for (int diskIdx : tower.diskIndices) {
            auto& disk = disks[diskIdx];
            


            int numberOfPresentDisk = disks.size();
            int diskSize = disk.size;

            if (disk.node) {
                // Calcoliamo la nuova posizione assoluta
                // Manteniamo X e Z della torre, cambiamo solo Y
                glm::vec3 newPos;
                
                newPos = glm::vec3(tower.position.x, (float)(((diskSize - 1.0f) - (tower.diskIndices.size()-index)) * (-DISK_HEIGHT)), tower.position.z);
                
                

                // Se c'è un disco selezionato (in aria), lo alziamo visivamente
                if (gameState == GameState::DISK_SELECTED && selectedDiskIndex == diskIdx) {
                    newPos.y += 100.0f; // Solleva il disco selezionato di 100 unità
                }

                disk.node->setPosition(newPos);
                std::cout << "disk POSITION : " << newPos.y << std::endl;
                std::cout << "disks SIZE : " << tower.diskIndices.size() << std::endl;
                std::cout << "disks SIZE : " << diskSize << std::endl;
            }

            index--;
            
        }
    }
}

bool HanoiGame::isValidMove(int diskIndex, int targetTower) {
    if (diskIndex == -1) return false;

    Disk& movingDisk = disks[diskIndex];
    Tower& destTower = towers[targetTower];

    // Non muovere sulla stessa torre
    if (movingDisk.currentTower == targetTower) return false;

    // Se la torre è vuota, ok
    if (destTower.diskIndices.empty()) return true;

    // Controllo dimensione
    int topDiskIndex = destTower.diskIndices.back();
    Disk& topDisk = disks[topDiskIndex];

    // TUA REGOLA: Disk_7 (Piccolo) può stare su Disk_1 (Grande).
    // Quindi: MovingDisk (es. 7) deve essere > TopDisk (es. 1)
    if (movingDisk.size < topDisk.size) {
        std::cout << "Mossa non valida! Non puoi mettere un disco piu' grande (es. Disk_1) su uno piu' piccolo (es. Disk_7)" << std::endl;
        return false;
    }

    return true;
}

bool HanoiGame::performMove(int diskIndex, int targetTower) {
    if (!isValidMove(diskIndex, targetTower)) {
        std::cout << "Invalid move!" << std::endl;
        return false;
    }
    
    Disk& disk = disks[diskIndex];
    int sourceTowerIdx = disk.currentTower;
    // Se il giocatore fa una nuova mossa, il futuro (redo) non esiste più
    while (!redoStack.empty()) {
        redoStack.pop();
    }

    // Registriamo l'azione corrente nello stack Undo
    MoveAction action;
    action.diskIndex = diskIndex;
    action.sourceTowerIdx = sourceTowerIdx;
    action.destTowerIdx = targetTower;
    undoStack.push(action);
    
    // Remove from source tower
    auto& sourceTower = towers[disk.currentTower];
    sourceTower.diskIndices.pop_back();
    
    // Add to target tower
    disk.currentTower = targetTower;
    auto& targetTowerObj = towers[targetTower];
    targetTowerObj.diskIndices.push_back(diskIndex);
    
    moveCount++;
    
    std::cout << "Moved disk size " << disk.size 
              << " from tower " << (disk.currentTower)  // Note: currentTower is now target
              << " to tower " << targetTower 
              << " (Move: " << moveCount << ")" << std::endl;
    
    // Update disk positions
    updateDiskPositions();
    
    // Update screen text
    Engine::setScreenText(getStatus());
    
    // Check win condition
    checkWinCondition();
    
    return true;
}

void HanoiGame::checkWinCondition() {
    if (towers[2].diskIndices.size() == NUM_DISKS) {
        gameState = GameState::GAME_WON;
        std::cout << "Congratulations! You solved the Tower of Hanoi in " 
                  << moveCount << " moves!" << std::endl;
        std::cout << "Minimum moves possible: " << minMovesRequired << std::endl;
        
        if (moveCount == minMovesRequired) {
            std::cout << "PERFECT SCORE! You solved it in the minimum number of moves!" << std::endl;
        }
    }
}

void HanoiGame::undo() {
    if (undoStack.empty()) {
        std::cout << "Niente da annullare!" << std::endl;
        return;
    }

    MoveAction lastMove = undoStack.top();
    undoStack.pop();

    // Logica inversa manuale (senza chiamare performMove per non incasinare lo stack)
    Disk& disk = disks[lastMove.diskIndex];

    // Rimuovi dalla torre attuale (che era la destinazione)
    towers[lastMove.destTowerIdx].diskIndices.pop_back();

    // Rimetti nella torre originale (sorgente)
    towers[lastMove.sourceTowerIdx].diskIndices.push_back(lastMove.diskIndex);
    disk.currentTower = lastMove.sourceTowerIdx;

    // Aggiungi alla redo stack
    redoStack.push(lastMove);

    moveCount--;
    updateDiskPositions();
    Engine::setScreenText(getStatus());
}

void HanoiGame::redo() {
    if (redoStack.empty()) {
        std::cout << "Niente da ripristinare (Redo stack vuoto)!" << std::endl;
        return;
    }

    MoveAction nextMove = redoStack.top();
    redoStack.pop();

    // Logica di ripristino della mossa
    Disk& disk = disks[nextMove.diskIndex];

    // 1. Rimuovi dalla torre sorgente (dove è tornato dopo l'undo)
    towers[nextMove.sourceTowerIdx].diskIndices.pop_back();

    // 2. Aggiungi alla torre destinazione (dove era andato in origine)
    towers[nextMove.destTowerIdx].diskIndices.push_back(nextMove.diskIndex);

    // 3. Aggiorna il riferimento della torre nel disco
    disk.currentTower = nextMove.destTowerIdx;

    // Aggiungi nuovamente alla undo stack (così si può annullare di nuovo)
    undoStack.push(nextMove);

    moveCount++;
    std::cout << "Redo: sposta " << disk.name << " su Torre " << (nextMove.destTowerIdx + 1) << std::endl;

    updateDiskPositions();
    Engine::setScreenText(getStatus());
}

/*
int HanoiGame::findDiskIndex(const std::string& diskName) {
    for (int i = 0; i < disks.size(); i++) {
        if (disks[i].name == diskName) {
            return i;
        }
    }
    return -1;
}
*/
/*
int HanoiGame::findTowerIndex(const std::string& towerName) {
    for (int i = 0; i < towers.size(); i++) {
        if (towers[i].name == towerName) {
            return i;
        }
    }
    return -1;
}
*/