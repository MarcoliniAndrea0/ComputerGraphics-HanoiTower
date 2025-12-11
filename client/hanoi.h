#pragma once

#include "engine.h"
#include "node.h"
#include <memory>
#include <vector>
#include <stack>
#include <glm/glm.hpp>

/**
 * @class HanoiGame
 * @brief Tower of Hanoi game implementation
 */
class LIB_API HanoiGame {
public:
    /**
     * @brief Initialize the game
     */
    static void init(std::shared_ptr<Node> rootNode);
    /**
     * @brief Reset the game to initial state
     */
    static void reset();
    
    /**
     * @brief Handle mouse click for game interaction
     * @param mouseX Mouse X coordinate
     * @param mouseY Mouse Y coordinate
     */
    //static void handleClick(int mouseX, int mouseY);
    
    /**
     * @brief Handle keyboard input for game control
     * @param key Key pressed
     */
    static void handleKey(unsigned char key);
    
    /**
     * @brief Get the current game status text
     * @return Status string for display
     */
    static std::string getStatus();
    
    /**
     * @brief Update game animation
     * @param deltaTime Time since last update
     */
    static void update(/*float deltaTime*/);
    
    /**
     * @brief Check if game is won
     * @return True if game is won
     */
    static bool isGameWon();
    
    /**
     * @brief Get current move count
     * @return Number of moves made
     */
    static int getMoveCount();
    
    /**
     * @brief Get minimum required moves
     * @return Minimum moves to solve
     */
    static int getMinMoves();
    
    /**
     * @brief Show game instructions
     */
    static void showHelp();
    
    /**
     * @brief Auto-solve the puzzle (for testing)
     */
    static void autoSolve();

    static void undo();
    static void redo();
    
private:
    // Game constants
    static const int NUM_DISKS;
    static const int NUM_TOWERS;
    static const float DISK_HEIGHT;
    
    // Game state
    enum class GameState {
        IDLE,
        DISK_SELECTED,
        GAME_WON
    };
    
    // Disk structure
    struct Disk {
        std::shared_ptr<Node> node; // Puntatore al nodo visuale reale
        int size;  // 1 (piccolo) a 7 (grande)
        int currentTower;
        std::string name;
        glm::vec3 originalPosition; // Per il reset
    };
    
    // Tower structure
    struct Tower {
        std::shared_ptr<Node> node; // Puntatore al nodo visuale reale
        std::vector<int> diskIndices;
        glm::vec3 position; // Posizione della base della torre
        std::string name;
    };

    struct MoveAction {
        int diskIndex;
        int sourceTowerIdx;
        int destTowerIdx;
    };
    
    // Private methods
    static void loadGameObjects(std::shared_ptr<Node> rootNode);
    static void updateDiskPositions();
    static bool isValidMove(int diskIndex, int targetTower);
    static bool performMove(int diskIndex, int targetTower);
    static void checkWinCondition();
    //static int findDiskIndex(const std::string& diskName);
    //static int findTowerIndex(const std::string& towerName);

    // Gestisce la logica di input per una specifica torre (sia da click che da tastiera)
    static void processTowerInput(int towerIndex);
    
    // Helper per trovare i nodi nella scena
    static std::shared_ptr<Node> findNode(std::shared_ptr<Node> root, const std::string& name);

    // Static game variables
    static GameState gameState;
    static std::vector<Disk> disks;
    static std::vector<Tower> towers;
    static int selectedDiskIndex;
    static int moveCount;
    static int minMovesRequired;
    static float animationTime;
    
    //gestiscono il ripristina mossa/annulla mossa
    static std::stack<MoveAction> undoStack;
    static std::stack<MoveAction> redoStack;

    // Teniamo traccia del rootNode per i reset
    static std::shared_ptr<Node> sceneRoot;
};