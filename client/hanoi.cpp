#include "hanoi.h"
#include <iostream>

static std::shared_ptr<Node> sceneRoot = nullptr;

// Initialize static constants
const int HanoiGame::NUM_DISKS = 4;
const int HanoiGame::NUM_TOWERS = 3;
const float HanoiGame::DISK_HEIGHT = 0.3f;
const float HanoiGame::DISK_RADIUS_BASE = 0.2f;
const float HanoiGame::DISK_RADIUS_INCREMENT = 0.15f;
const float HanoiGame::TOWER_HEIGHT = 3.0f;
const float HanoiGame::TOWER_RADIUS = 0.1f;
const float HanoiGame::TOWER_SPACING = 3.0f;
const float HanoiGame::ANIMATION_DURATION = 0.5f;

// Disk colors (RGB)
const glm::vec3 HanoiGame::DISK_COLORS[7] = {
    glm::vec3(1.0f, 0.0f, 0.0f),    // Red
    glm::vec3(1.0f, 0.5f, 0.0f),    // Orange
    glm::vec3(1.0f, 1.0f, 0.0f),    // Yellow
    glm::vec3(0.0f, 1.0f, 0.0f),    // Green
    glm::vec3(0.0f, 1.0f, 1.0f),    // Cyan
    glm::vec3(0.0f, 0.0f, 1.0f),    // Blue
    glm::vec3(0.8f, 0.0f, 1.0f)     // Purple
};

// Initialize static variables
HanoiGame::GameState HanoiGame::gameState = GameState::IDLE;
std::vector<HanoiGame::Disk> HanoiGame::disks;
std::vector<HanoiGame::Tower> HanoiGame::towers;
int HanoiGame::selectedDiskIndex = -1;
int HanoiGame::moveCount = 0;
int HanoiGame::minMovesRequired = 0;
float HanoiGame::animationTime = 0.0f;

void HanoiGame::init(std::shared_ptr<Node> root) {
    sceneRoot = root;
    init(); // Call original init
}

void HanoiGame::init() {
    std::cout << "Initializing Tower of Hanoi game..." << std::endl;
    
    // Reset game state
    gameState = GameState::IDLE;
    selectedDiskIndex = -1;
    moveCount = 0;
    minMovesRequired = (1 << NUM_DISKS) - 1; // 2^n - 1
    animationTime = 0.0f;
    
    // Clear existing containers
    disks.clear();
    towers.clear();
    
    // Create game elements
    createTowers();
    createDisks();
    
    std::cout << "Tower of Hanoi initialized. Minimum moves required: " 
              << minMovesRequired << std::endl;
}

void HanoiGame::createTowers() {
    for (int i = 0; i < NUM_DISKS; i++) {
        Disk disk;
        disk.size = NUM_DISKS - i; // Largest disk first (size 7), smallest last (size 1)
        disk.currentTower = 0; // All disks start on first tower
        disk.baseColor = DISK_COLORS[i];
        disk.name = "Disk_" + std::to_string(disk.size);
        
        // Create disk node
        disk.node = std::make_shared<Node>(disk.name);
        
        // ADD TO SCENE
        if (sceneRoot) {
            sceneRoot->addChild(disk.node);
        }
        
        disks.push_back(disk);
        
        // Add disk to first tower
        towers[0].diskIndices.push_back(i);
        
        std::cout << "Created disk: " << disk.name 
                  << " with color: " << disk.baseColor.r << ", "
                  << disk.baseColor.g << ", " << disk.baseColor.b << std::endl;
    }
    
    // Update initial positions
    updateDiskPositions();
}

void HanoiGame::createDisks() {
    for (int i = 0; i < NUM_DISKS; i++) {
        Disk disk;
        disk.size = NUM_DISKS - i; // Largest disk first (size 7), smallest last (size 1)
        disk.currentTower = 0; // All disks start on first tower
        disk.baseColor = DISK_COLORS[i];
        disk.name = "Disk_" + std::to_string(disk.size);
        
        // Create disk node
        disk.node = std::make_shared<Node>(disk.name);
        // Note: You'll need to add mesh and set position based on your scene structure
        
        disks.push_back(disk);
        
        // Add disk to first tower
        towers[0].diskIndices.push_back(i);
        
        std::cout << "Created disk: " << disk.name 
                  << " with color: " << disk.baseColor.r << ", "
                  << disk.baseColor.g << ", " << disk.baseColor.b << std::endl;
    }
    
    // Update initial positions
    updateDiskPositions();
}

void HanoiGame::reset() {
    std::cout << "Resetting Tower of Hanoi game..." << std::endl;
    init();
}

void HanoiGame::handleClick(int mouseX, int mouseY) {
    if (gameState == GameState::GAME_WON) {
        std::cout << "Game already won! Press 'R' to restart." << std::endl;
        return;
    }
    
    // Get clicked node
    auto clickedNode = Engine::getNodeByClick(mouseX, mouseY);
    if (!clickedNode) {
        std::cout << "No disk or tower clicked" << std::endl;
        return;
    }
    
    std::string nodeName = clickedNode->getName();
    std::cout << "Clicked on: " << nodeName << std::endl;
    
    // Check if a disk was clicked
    if (nodeName.find("Disk_") == 0) {
        int diskIndex = findDiskIndex(nodeName);
        
        if (diskIndex == -1) {
            std::cout << "Disk not found!" << std::endl;
            return;
        }
        
        if (gameState == GameState::IDLE) {
            // First click - select disk
            selectedDiskIndex = diskIndex;
            gameState = GameState::DISK_SELECTED;
            
            std::cout << "Selected disk size " << disks[selectedDiskIndex].size << std::endl;
            Engine::setScreenText("Selected disk " + std::to_string(disks[selectedDiskIndex].size) + 
                                 " - Click target tower");
        }
        else if (gameState == GameState::DISK_SELECTED) {
            // Second click on another disk - select new disk
            selectedDiskIndex = diskIndex;
            std::cout << "Selected disk size " << disks[selectedDiskIndex].size << std::endl;
            Engine::setScreenText("Selected disk " + std::to_string(disks[selectedDiskIndex].size) + 
                                 " - Click target tower");
        }
    }
    // Check if a tower was clicked
    else if (nodeName.find("Tower_") == 0 && gameState == GameState::DISK_SELECTED) {
        int targetTower = findTowerIndex(nodeName);
        
        if (targetTower == -1) {
            std::cout << "Tower not found!" << std::endl;
            return;
        }
        
        std::cout << "Attempting to move disk to tower " << targetTower << std::endl;
        
        // Attempt to move the selected disk
        if (performMove(selectedDiskIndex, targetTower)) {
            // Move successful, reset selection
            selectedDiskIndex = -1;
            gameState = GameState::IDLE;
        }
        else {
            Engine::setScreenText("Invalid move! Try again");
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

void HanoiGame::update(float deltaTime) {
    if (animationTime > 0) {
        animationTime -= deltaTime;
        // Update animation state here if needed
    }
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
    std::cout << "- H: Show this help" << std::endl;
    std::cout << "- ESC: Exit game" << std::endl;
    std::cout << "Minimum moves required: " << minMovesRequired << std::endl;
    std::cout << "=================================" << std::endl;
    
    Engine::setScreenText("Goal: Move all 7 disks to right tower. Min moves: " + 
                         std::to_string(minMovesRequired));
}

// Private helper methods

void HanoiGame::updateDiskPositions() {
    // Clear all tower disk indices
    for (auto& tower : towers) {
        tower.diskIndices.clear();
    }
    
    // Rebuild tower disk indices
    for (int i = 0; i < disks.size(); i++) {
        towers[disks[i].currentTower].diskIndices.push_back(i);
    }
    
    // Update visual positions
    for (int towerIdx = 0; towerIdx < NUM_TOWERS; towerIdx++) {
        auto& tower = towers[towerIdx];
        float currentHeight = 0.0f;
        
        for (int diskIdx : tower.diskIndices) {
            auto& disk = disks[diskIdx];
            glm::vec3 targetPosition = tower.position + glm::vec3(0.0f, currentHeight, 0.0f);
            
            // disk.node->setPosition(targetPosition);
            
            currentHeight += DISK_HEIGHT;
        }
    }
}

bool HanoiGame::isValidMove(int diskIndex, int targetTower) {
    if (diskIndex < 0 || diskIndex >= disks.size()) return false;
    if (targetTower < 0 || targetTower >= NUM_TOWERS) return false;
    
    Disk& disk = disks[diskIndex];
    
    // Can't move to the same tower
    if (disk.currentTower == targetTower) return false;
    
    // Check if disk is on top of its tower
    auto& sourceTower = towers[disk.currentTower];
    if (sourceTower.diskIndices.empty() || 
        sourceTower.diskIndices.back() != diskIndex) {
        return false;
    }
    
    // Check if target tower is empty or has a larger disk on top
    auto& targetTowerObj = towers[targetTower];
    if (targetTowerObj.diskIndices.empty()) {
        return true;
    }
    
    int topDiskIndex = targetTowerObj.diskIndices.back();
    return disks[topDiskIndex].size > disk.size;
}

bool HanoiGame::performMove(int diskIndex, int targetTower) {
    if (!isValidMove(diskIndex, targetTower)) {
        std::cout << "Invalid move!" << std::endl;
        return false;
    }
    
    Disk& disk = disks[diskIndex];
    
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

int HanoiGame::findDiskIndex(const std::string& diskName) {
    for (int i = 0; i < disks.size(); i++) {
        if (disks[i].name == diskName) {
            return i;
        }
    }
    return -1;
}

int HanoiGame::findTowerIndex(const std::string& towerName) {
    for (int i = 0; i < towers.size(); i++) {
        if (towers[i].name == towerName) {
            return i;
        }
    }
    return -1;
}