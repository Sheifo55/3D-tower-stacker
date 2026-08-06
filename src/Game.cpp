#include "Game.h"
#include "InputHandler.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>

template <typename T>
int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

glm::vec3 hsv2rgb(glm::vec3 c) {
    glm::vec4 K = glm::vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(c.x) + glm::vec3(K.x, K.y, K.z)) * 6.0f - glm::vec3(K.w));
    return c.z * glm::mix(glm::vec3(K.x), glm::clamp(p - glm::vec3(K.x), 0.0f, 1.0f), c.y);
}

Game::Game() : state(GameState::START_SCREEN), score(0), blockHeight(0.3f),
               oscillationTime(0.0f), oscillationSpeed(2.0f), oscillationAmplitude(3.0f),
               movingAlongX(true), camera(nullptr) {}

void Game::init() {
    towerBlocks.clear();
    scrapPieces.clear();
    score = 0;
    oscillationSpeed = 2.0f;
    
    // Add base foundation block
    Block baseBlock;
    baseBlock.position = glm::vec3(0.0f, 0.0f, 0.0f);
    baseBlock.size = glm::vec3(3.0f, blockHeight, 3.0f);
    baseBlock.color = glm::vec3(0.25f, 0.28f, 0.35f);
    towerBlocks.push_back(baseBlock);
    
    movingAlongX = true;
    spawnNextBlock();
}

void Game::setCamera(Camera* cam) {
    camera = cam;
}

void Game::startGame() {
    state = GameState::PLAYING;
    init();
    if (camera) {
        camera->setTargetY(0.0f);
    }
}

glm::vec3 Game::getColorForLevel(int level) const {
    float hue = fmod(level * 30.0f, 360.0f) / 360.0f; // 30° shift per level
    return hsv2rgb(glm::vec3(hue, 0.85f, 0.98f));
}

void Game::spawnNextBlock() {
    Block prev = towerBlocks.back();
    currentBlock.size = prev.size;
    currentBlock.position = prev.position;
    currentBlock.position.y += blockHeight;
    currentBlock.color = getColorForLevel(score + 1);
    
    oscillationTime = 0.0f;
    if (movingAlongX) {
        currentBlock.position.x = prev.position.x - oscillationAmplitude;
    } else {
        currentBlock.position.z = prev.position.z - oscillationAmplitude;
    }
}

void Game::update(float deltaTime) {
    if (state == GameState::PLAYING) {
        updateOscillation(deltaTime);
    }
    updateScrapPieces(deltaTime);
    
    for (auto& block : towerBlocks) {
        block.update(deltaTime);
    }
    currentBlock.update(deltaTime);
}

void Game::updateOscillation(float deltaTime) {
    oscillationTime += deltaTime * oscillationSpeed;
    float offset = sin(oscillationTime) * oscillationAmplitude;
    
    Block prev = towerBlocks.back();
    if (movingAlongX) {
        currentBlock.position.x = prev.position.x + offset;
    } else {
        currentBlock.position.z = prev.position.z + offset;
    }
}

void Game::updateScrapPieces(float deltaTime) {
    for (auto it = scrapPieces.begin(); it != scrapPieces.end(); ) {
        it->update(deltaTime);
        if (it->isOffScreen()) {
            it = scrapPieces.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::dropBlock() {
    Block prev = towerBlocks.back();
    
    int axisIndex = movingAlongX ? 0 : 2; // 0 for X, 2 for Z
    float currentPos = movingAlongX ? currentBlock.position.x : currentBlock.position.z;
    float prevPos = movingAlongX ? prev.position.x : prev.position.z;
    float currentSize = movingAlongX ? currentBlock.size.x : currentBlock.size.z;
    
    float offset = currentPos - prevPos;
    float overlap = currentSize - std::abs(offset);
    
    if (std::abs(offset) < 0.05f) {
        // Perfect placement bonus!
        offset = 0.0f;
        overlap = currentSize;
        currentBlock.flashTimer = 0.5f; // Flash for 0.5s
    } else if (overlap <= 0.001f) {
        state = GameState::GAME_OVER;
        return;
    }
    
    Block placedBlock = currentBlock;
    
    if (movingAlongX) {
        placedBlock.position.x = prev.position.x + offset / 2.0f;
        placedBlock.size.x = overlap;
    } else {
        placedBlock.position.z = prev.position.z + offset / 2.0f;
        placedBlock.size.z = overlap;
    }
    
    towerBlocks.push_back(placedBlock);
    
    // Create Scrap Piece only if not perfect
    if (std::abs(offset) > 0.0f) {
        ScrapPiece scrap;
        scrap.color = placedBlock.color;
        scrap.size = currentBlock.size;
        scrap.position = currentBlock.position;
    
    float scrapSize = currentSize - overlap;
    if (movingAlongX) {
        scrap.size.x = scrapSize;
        scrap.position.x = currentPos + sign(offset) * (overlap / 2.0f);
    } else {
        scrap.size.z = scrapSize;
        scrap.position.z = currentPos + sign(offset) * (overlap / 2.0f);
    }
    
        scrap.velocity = glm::vec3((float(rand() % 100) / 100.0f - 0.5f) * 2.0f, -2.0f, (float(rand() % 100) / 100.0f - 0.5f) * 2.0f);
        scrap.rotationAxis = glm::normalize(glm::vec3(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f));
        if (glm::length(scrap.rotationAxis) < 0.1f) scrap.rotationAxis = glm::vec3(1, 0, 0);
        scrap.rotationSpeed = 90.0f + float(rand() % 180);
        scrap.rotationAngle = 0.0f;
        
        scrapPieces.push_back(scrap);
    }
    
    score++;
    if (camera) {
        camera->setTargetY(placedBlock.position.y);
    }
    
    movingAlongX = !movingAlongX;
    oscillationSpeed = std::min(6.0f, oscillationSpeed + 0.05f);
    
    spawnNextBlock();
}

void Game::handleInput() {
    if (InputHandler::isQuitPressed()) {
        // Handled outside
    }
    
    if (state == GameState::START_SCREEN) {
        if (InputHandler::isDropPressed()) {
            startGame();
        }
    } else if (state == GameState::PLAYING) {
        if (InputHandler::isDropPressed()) {
            dropBlock();
        }
    } else if (state == GameState::GAME_OVER) {
        if (InputHandler::isRestartPressed()) {
            startGame();
        }
    }
}

void Game::render(const Renderer& renderer, Shader& shader, TextRenderer& textRenderer, Shader& textShader, int screenWidth, int screenHeight) {
    shader.use();
    
    // Set global light uniforms
    shader.setVec3("lightDir", glm::normalize(glm::vec3(1.0f, 1.0f, 0.5f)));
    shader.setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.9f));
    
    if (camera) {
        shader.setVec3("viewPos", camera->getPosition());
    }

    // Render tower blocks (default material)
    shader.setFloat("ambientStrength", 0.32f);
    shader.setFloat("specularStrength", 0.55f);
    shader.setFloat("shininess", 32.0f);

    for (size_t i = 0; i < towerBlocks.size(); ++i) {
        // Frustum culling: don't render blocks far below the camera
        if (camera && towerBlocks[i].position.y < camera->getPosition().y - 30.0f) {
            continue;
        }

        if (i == 0) {
            // Base platform material
            shader.setFloat("ambientStrength", 0.35f);
            shader.setFloat("specularStrength", 0.7f);
            shader.setFloat("shininess", 48.0f);
        } else {
            // Reset to default material
            shader.setFloat("ambientStrength", 0.32f);
            shader.setFloat("specularStrength", 0.55f);
            shader.setFloat("shininess", 32.0f);
        }
        
        // Flash effect for perfect placement
        glm::vec3 blockColor = towerBlocks[i].color;
        if (towerBlocks[i].flashTimer > 0.0f) {
            float flashAmount = towerBlocks[i].flashTimer / 0.5f;
            blockColor = glm::mix(blockColor, glm::vec3(1.0f), flashAmount); // Blend towards white
        }

        renderer.drawCube(shader, towerBlocks[i].getModelMatrix(), blockColor);
    }
    
    if (state == GameState::PLAYING) {
        glm::vec3 blockColor = currentBlock.color;
        if (currentBlock.flashTimer > 0.0f) {
            float flashAmount = currentBlock.flashTimer / 0.5f;
            blockColor = glm::mix(blockColor, glm::vec3(1.0f), flashAmount);
        }
        renderer.drawCube(shader, currentBlock.getModelMatrix(), blockColor);
    }
    
    // Scrap pieces have reduced specular (more matte as they fall)
    shader.setFloat("specularStrength", 0.1f);
    shader.setFloat("shininess", 8.0f);
    for (const auto& scrap : scrapPieces) {
        renderer.drawCube(shader, scrap.getModelMatrix(), scrap.color);
    }

    // HUD & Text Rendering
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float centerX = screenWidth / 2.0f - 100.0f; // Approx centering
    float topY = screenHeight - 100.0f;
    float midY = screenHeight / 2.0f;
    float botY = screenHeight / 4.0f;
    
    glm::vec3 darkText(0.12f, 0.15f, 0.22f);
    glm::vec3 grayText(0.40f, 0.43f, 0.50f);
    glm::vec3 redText(0.88f, 0.15f, 0.20f);

    if (state == GameState::START_SCREEN) {
        textRenderer.renderText(textShader, "3D TOWER STACKER", centerX - 60.0f, topY, 1.5f, darkText);
        textRenderer.renderText(textShader, "Press SPACE to Start", centerX - 50.0f, midY, 1.0f, grayText);
    }
    else if (state == GameState::PLAYING) {
        textRenderer.renderText(textShader, "Score: " + std::to_string(score), 10.0f, screenHeight - 40.0f, 1.0f, darkText);
    }
    else if (state == GameState::GAME_OVER) {
        textRenderer.renderText(textShader, "GAME OVER", centerX, topY, 2.0f, redText);
        textRenderer.renderText(textShader, "Score: " + std::to_string(score), centerX, midY, 1.5f, darkText);
        textRenderer.renderText(textShader, "Press R to Restart", centerX - 20.0f, botY, 1.0f, grayText);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

GameState Game::getState() const {
    return state;
}

int Game::getScore() const {
    return score;
}
