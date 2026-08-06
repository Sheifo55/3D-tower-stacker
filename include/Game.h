#pragma once

#include "Block.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "TextRenderer.h"
#include <vector>
#include <string>

enum class GameState {
    START_SCREEN,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game();

    void init();
    void update(float deltaTime);
    void render(const Renderer& renderer, Shader& shader, TextRenderer& textRenderer, Shader& textShader, int screenWidth, int screenHeight);
    void handleInput();

    GameState getState() const;
    int getScore() const;
    void setCamera(Camera* cam);

private:
    GameState state;
    int score;

    // Tower data
    std::vector<Block> towerBlocks;  // Placed blocks (bottom to top)
    Block currentBlock;               // The moving block
    float blockHeight;                // Constant (e.g., 0.3f)

    // Oscillation
    float oscillationTime;
    float oscillationSpeed;           // Increases with score
    float oscillationAmplitude;       // Range of movement (e.g., 3.0f)
    bool movingAlongX;                // Alternates: true=X, false=Z

    // Scrap pieces
    std::vector<ScrapPiece> scrapPieces;

    // Camera reference
    Camera* camera;

    // Private methods
    void startGame();
    void dropBlock();
    void spawnNextBlock();
    void updateOscillation(float deltaTime);
    void updateScrapPieces(float deltaTime);
    glm::vec3 getColorForLevel(int level) const;
};
