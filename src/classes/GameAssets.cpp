#include "GameAssets.hpp"

/**
 * @brief Calculates Image size accordingly to screen resolution.
 * 
 */
GameAssets::GameAssets() {
    // Using (0, 0) initializes a window that matches the current monitor screen resolution
    InitWindow(0, 0, "Initializing...");

    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int lowest = height;

    CloseWindow();

    // Vertical monitor
    if (width < height)
        lowest = width;
    
    float scale = (float) lowest / (float) SCREEN_BASE_HEIGHT;

    _blockSize =       scale * BLOCK_BASE_SIZE;
    _borderSize =      scale * BORDER_BASE_SIZE;
    _topBorderSize =   scale * TOP_BORDER_BASE_SIZE;
    _fontSize =        scale * FONT_BASE_SIZE;    
    _buttonWidthSize = scale * BUTTON_WIDTH_BASE_SIZE;   
    _lineThickness =   std::roundf(scale * LINE_BASE_THICKNESS);
}

/**
 * @brief Unloads all Fonts and Textures
 * 
 */
GameAssets::~GameAssets() {
    UnloadFont(_font);

    UnloadTexture(_block);
    UnloadTexture(_flag);
    UnloadTexture(_mine);
    UnloadTexture(_button);
}

/**
 * @brief Loads all Fonts and Textures. 
 * Can ONLY be called after InitWindow.
 * 
 */
void GameAssets::loadAssets() {
    Image baseBlock = LoadImage("./assets/images/block.png");
    Image baseFlag = LoadImage("./assets/images/flag.png");
    Image baseMine = LoadImage("./assets/images/mine.png");
    Image baseButton = LoadImage("./assets/images/button.png");

    ImageResize(&baseBlock, _blockSize, _blockSize);
    ImageResize(&baseFlag, _blockSize, _blockSize);
    ImageResize(&baseMine, _blockSize, _blockSize);
    ImageResize(&baseButton, _buttonWidthSize, _blockSize);

    // Load font and textures (resized)
    _font = LoadFont("./assets/fonts/Pixellari.otf");

    _block = LoadTextureFromImage(baseBlock);
    _flag = LoadTextureFromImage(baseFlag);
    _mine = LoadTextureFromImage(baseMine);
    _button = LoadTextureFromImage(baseButton);

    UnloadImage(baseBlock);
    UnloadImage(baseFlag);
    UnloadImage(baseMine);
    UnloadImage(baseButton);
}

/**
 * @brief Returns the text font 
 * 
 */
Font& GameAssets::getFont() {
    return _font;
}

/**
 * @brief Returns the block texture
 * 
 */
Texture2D& GameAssets::getBlock() {
    return _block;
}

/**
 * @brief Returns the flag texture
 * 
 */
Texture2D& GameAssets::getFlag() {
    return _flag;
}

/**
 * @brief Returns the mine texture
 * 
 */
Texture2D& GameAssets::getMine() {
    return _mine;
}

/**
 * @brief Returns the button texture
 * 
 */
Texture2D& GameAssets::getButton() {
    return _button;
}

/**
 * @brief Returns the block size
 * 
 */
float GameAssets::getBlockSize() {
    return _blockSize;
}

/**
 * @brief Returns the border size
 * 
 */
float GameAssets::getBorderSize() {
    return _borderSize;
}

/**
 * @brief Returns the top border size
 * 
 */
float GameAssets::getTopBorderSize() {
    return _topBorderSize;
}

/**
 * @brief Returns the font size
 * 
 */
float GameAssets::getFontSize() {
    return _fontSize;
}

/**
 * @brief Returns the button width size
 * 
 */
float GameAssets::getButtonWidthSize() {
    return _buttonWidthSize;
}

/**
 * @brief Returns the line thickness
 * 
 */
float GameAssets::getLineThickness() {
    return _lineThickness;
}