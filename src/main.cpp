#include <iostream>

#include "GameAssets.hpp"
#include "GameData.hpp"

// Target FPS
#define TARGET_FPS      60

// Board background colors
#define BACK_COLOR      CLITERAL(Color){192, 192, 192, 255}
#define LINE_COLOR      CLITERAL(Color){128, 128, 128, 255}

// Other Colors
#define WIN_COLOR       CLITERAL(Color){0, 128, 0, 255}
#define LOSE_COLOR      CLITERAL(Color){255, 0, 0, 255}
#define UI_TEXT_COLOR   CLITERAL(Color){0, 0, 128, 255}

/**
 * @brief Draws a text centralized on a given position.
 * 
 * @param assets Assets class with the font to be used.
 * @param text Desired text.
 * @param xCenter Desired position x coordinate.
 * @param yCenter Desired position y coordinate.
 * @param fontSize Desired font size.
 * @param color Desired color.
 */
void DrawTextExCenter(GameAssets* &assets, std::string text, float xCenter, float yCenter, float fontSize, Color color) {
    // Initial position
    Vector2 textPosition = {xCenter, yCenter};

    // Offset by text size
    Vector2 textSize = MeasureTextEx(assets->getFont(), text.c_str(), fontSize, 0); 
    
    textPosition.x -= textSize.x / 2.0f;
    textPosition.y -= textSize.y / 2.6f; // NOTE: Pixellari has a weird Y offset for some reason

    DrawTextEx(assets->getFont(), text.c_str(), textPosition, fontSize, 0, color);
}

/**
 * @brief Converts mouse position to a coordinate that matches the board vector. 
 * So that it can be used simply with board[row][col].
 * 
 * @param assets Assets class.
 * @param game Game class.
 * @return Converted coordinate. Returns (-1, -1) if the mouse is not hovering over the board.
 */
Coordinate getCoordFromMouse(GameAssets* &assets, GameData* &game) {
    int x = GetMouseX();
    int y = GetMouseY();

    const int rightBoardLimit = assets->getBorderSize() + game->getCols() * assets->getBlockSize();
    const int bottomBoardLimit = assets->getTopBorderSize() + game->getRows() * assets->getBlockSize();

    if (x >= assets->getBorderSize() && x <= rightBoardLimit) {
        if (y >= assets->getTopBorderSize() && y <= bottomBoardLimit) {
            x = (x - assets->getBorderSize()) / assets->getBlockSize();
            y = (y - assets->getTopBorderSize()) / assets->getBlockSize();

            // Since vectors start indexing at 0, avoid equal rows/cols caused by the mouse hovering over the board edge.
            if (y == game->getRows())
                y--;

            if (x == game->getCols())
                x--;

            return Coordinate(y, x);
        }
    }

    return Coordinate();
}

/**
 * @brief Handles mouse clicks to reveal/flag blocks. Also checks win/lose scenarios and game resets.
 * 
 * @param assets Assets class.
 * @param game Game class.
 */
void handleEvents(GameAssets* &assets, GameData* &game) {
    // Reset
    if (IsKeyPressed(KEY_R))
        game->resetGame();

    // Check win scenario
    int hiddenBlocks = 0;
    
    for (auto &line : game->getBoard())
        for (auto &block : line)
            hiddenBlocks += block->isHidden();

    // Game over by revealing all non-mine blocks (win)
    if (hiddenBlocks == game->getMines())
        game->setGameOver();                        

    // Process mouse click
    Coordinate coord = getCoordFromMouse(assets, game);

    if (coord.getRow() == -1 || game->isGameOver() == true)
        return;

    Block* block = game->getBoard()[coord.getRow()][coord.getCol()];

    // Reveal block
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (game->isFirstClick()) {
            if (block->getMineCount() == MINE_BLOCK) 
                block = game->moveMine(coord.getRow(), coord.getCol());
            
            game->setFirstClickUsed();
        }

        bool shouldContinue = block->reveal(game->getBoard(), game->getFlags());

        // Game over by revealing a mine (lose)
        if (shouldContinue == false)
            game->setGameOver(coord.getRow(), coord.getCol()); 
    }
    
    // Toggle flag
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && block->isHidden()) {
        if (game->getFlags() > 0)
            block->toggleFlag(game->getFlags());
        else if (block->isFlagged())
            block->toggleFlag(game->getFlags());
    }
}

/**
 * @brief Draws the block on the correct position. 
 * Draws mines and counters underneath, then draws a cover and a possible flag if the block is hidden.
 * 
 * @param assets Assets class.
 * @param game Game class.
 * @param row Block row.
 * @param col Block column.
 */
void drawBlock(GameAssets* &assets, GameData* &game, int &row, int &col) {
    // Calculate x,y position 
    const float xPos = assets->getBorderSize() + col * assets->getBlockSize();
    const float yPos = assets->getTopBorderSize() + row * assets->getBlockSize();

    const float xPosCenter = xPos + assets->getBlockSize() / 2;
    const float yPosCenter = yPos + assets->getBlockSize() / 2;

    const int mineCount = game->getBoard()[row][col]->getMineCount();
    
    // Draw block background
    if (game->isGameOver() && game->getGameOverMine() == Coordinate(row, col))
        DrawRectangle(xPos, yPos, assets->getBlockSize(), assets->getBlockSize(), LOSE_COLOR);
    else
        DrawRectangle(xPos, yPos, assets->getBlockSize(), assets->getBlockSize(), BACK_COLOR);

    DrawRectangleLinesEx((Rectangle){xPos, yPos, assets->getBlockSize(), assets->getBlockSize()}, assets->getLineThickness(), LINE_COLOR);

    // Draw mine
    if (mineCount == MINE_BLOCK)
        DrawTexture(assets->getMine(), xPos, yPos, WHITE);

    // Draw mine count
    if (mineCount > 0)
        DrawTextExCenter(assets, std::to_string(mineCount), xPosCenter, yPosCenter, assets->getFontSize(), game->getBoard()[row][col]->getNumberColor());
        
    // Draw block cover
    if (game->getBoard()[row][col]->isHidden())
        DrawTexture(assets->getBlock(), xPos, yPos, WHITE);
    
    if (game->getBoard()[row][col]->isFlagged())
        DrawTexture(assets->getFlag(), xPos, yPos, WHITE);
}

/**
 * @brief Draws game info such as flags counter, time elapsed and win/lose prompts.
 * 
 * @param assets Assets class.
 * @param game Game class.
 */
void drawGameInfo(GameAssets* &assets, GameData* &game) {
    // Initial text position
    Vector2 textPosition = {assets->getBorderSize(), assets->getBorderSize()};

    // Flag amount (aligned to left border)
    std::ostringstream flags; 
    flags << std::setw(3) << std::setfill('0') << game->getFlags();

    DrawTextEx(assets->getFont(), flags.str().c_str(), textPosition, assets->getFontSize(), 0, UI_TEXT_COLOR);

    // Time (aligned to right border)
    std::string time = game->getElapsedTime();
    Vector2 textSize = MeasureTextEx(assets->getFont(), "00:00:00", assets->getFontSize(), 0); 
    textPosition = {assets->getBorderSize() + game->getCols() * assets->getBlockSize() - textSize.x, assets->getBorderSize()};

    DrawTextEx(assets->getFont(), time.c_str(), textPosition, assets->getFontSize(), 0, UI_TEXT_COLOR);

    // Win or Lose (aligned to left border)
    textPosition = {assets->getBorderSize(), 2 * assets->getBorderSize()};

    if (game->isGameOver()) {
        if (game->getGameOverMine() == Coordinate(-1, -1))  // Game over was not caused by a mine
            DrawTextEx(assets->getFont(), "YOU WIN, YAY! :)", textPosition, assets->getFontSize(), 0, WIN_COLOR);
        else
            DrawTextEx(assets->getFont(), "GAME OVER! :(", textPosition, assets->getFontSize(), 0, LOSE_COLOR);
    }
}

int main() {
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL);
    SetRandomSeed(2017);

    static uint64_t frameCounter = 0;

    // Initialize game assets, calculates GUI elements size based on current screen resolution
    GameAssets* assets = new GameAssets();

    // Initialize general game data and board
    // Beginner:        9x9 with 10 mines
    // Intermediate:    16x16 with 40 mines
    // Expert:          16x30 with 99 mines
    GameData* game = new GameData(9, 9, 10);

    const int width = assets->getBorderSize() * 2 + game->getCols() * assets->getBlockSize();
    const int height = assets->getBorderSize() * 1 + game->getRows() * assets->getBlockSize() + assets->getTopBorderSize();

    InitWindow(width, height, "Gab's Minesweeper v.1.0");

    assets->loadAssets();

    // Main game loop
    while (WindowShouldClose() == false) {    
        if (game->isGameOver() == false && game->isFirstClick() == false && frameCounter++ == TARGET_FPS) {
            frameCounter = 0;
            game->addSecond();
        }

        handleEvents(assets, game);

        BeginDrawing();

            ClearBackground(BACK_COLOR);

            for (int row = 0; row < game->getRows(); row++)
                for (int col = 0; col < game->getCols(); col++) 
                    drawBlock(assets, game, row, col);

            drawGameInfo(assets, game);
                
        EndDrawing();
    }

    delete game;
    delete assets;

    CloseWindow();

    return 0;
}