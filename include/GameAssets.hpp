#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include <cmath>
#include <iostream>

#include "raylib.h"

// 4K screen height
#define SCREEN_BASE_HEIGHT      2160                        

// GUI base size for 4K screens
// NOTE: Images will be resized to fit resolutions below 4K correctly
#define BLOCK_BASE_SIZE         75
#define BORDER_BASE_SIZE        75                          // All around border
#define TOP_BORDER_BASE_SIZE    (BORDER_BASE_SIZE + 195)    // Extra space for top border
#define FONT_BASE_SIZE          (BLOCK_BASE_SIZE - 9)
#define BUTTON_WIDTH_BASE_SIZE  450
#define LINE_BASE_THICKNESS     3.0f                        // Lines between blocks

class GameAssets {
    private:
        Font _font;
        Texture2D _block;
        Texture2D _flag;
        Texture2D _mine;
        Texture2D _button;

        int _blockSize;
        int _borderSize;
        int _topBorderSize;
        int _fontSize;
        int _buttonWidthSize;
        float _lineThickness;

    public:
        GameAssets();
        ~GameAssets();

        void loadAssets();
        
        Font& getFont();
        Texture2D& getBlock();
        Texture2D& getFlag();
        Texture2D& getMine();
        Texture2D& getButton();

        float getBlockSize();
        float getBorderSize();
        float getTopBorderSize();
        float getFontSize();
        float getButtonWidthSize();
        float getLineThickness();
};

#endif