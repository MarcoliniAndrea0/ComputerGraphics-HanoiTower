#pragma once

#include "engine.h"
#include <memory>
#include <vector>
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
    static void init();
    
    /**
     * @brief Reset the game to initial state
     */
    static void reset();
    
    /**
     * @brief Handle mouse click for game interaction
     * @param mouseX Mouse X coordinate
     * @param mouseY Mouse Y coordinate
     */
    static void handleClick(int mouseX, int mouseY);
    
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
    static void update(float deltaTime);
    
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
    
private:
    // Game constants
    static const int NUM_DISKS;
    static const int NUM_TOWERS;
    static const float DISK_HEIGHT;
    static const float DISK_RADIUS_BASE;
    static const float DISK_RADIUS_INCREMENT;
    static const float TOWER_HEIGHT;
    static const float TOWER_RADIUS;
    static const float TOWER_SPACING;
    
    // Game state
    enum class GameState {
        IDLE,
        DISK_SELECTED,
        GAME_WON
    };
    
    // Disk structure
    struct Disk {
        std::shared_ptr<Node> node;
        int size;  // 1 (smallest) to 7 (largest)
        int currentTower;
        glm::vec3 baseColor;
        std::string name;
    };
    
    // Tower structure
    struct Tower {
        std::shared_ptr<Node> node;
        std::vector<int> diskIndices; // indices of disks on this tower (top is last)
        glm::vec3 position;
        std::string name;
    };
    
    // Private methods
    static void createTowers();
    static void createDisks();
    static void updateDiskPositions();
    static bool isValidMove(int diskIndex, int targetTower);
    static bool performMove(int diskIndex, int targetTower);
    static void checkWinCondition();
    static int findDiskIndex(const std::string& diskName);
    static int findTowerIndex(const std::string& towerName);
    
    // Static game variables
    static GameState gameState;
    static std::vector<Disk> disks;
    static std::vector<Tower> towers;
    static int selectedDiskIndex;
    static int moveCount;
    static int minMovesRequired;
    static float animationTime;
    static const float ANIMATION_DURATION;
    
    // Disk colors
    static const glm::vec3 DISK_COLORS[7];
};