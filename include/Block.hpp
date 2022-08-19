#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include "raylib.h"
#include "Coordinate.hpp"

// Mine block count
#define MINE_BLOCK      -1  

// Number colors
#define ONE_COLOR       CLITERAL(Color){0, 0, 255, 255}
#define TWO_COLOR       CLITERAL(Color){0, 128, 0, 255}
#define THREE_COLOR     CLITERAL(Color){255, 0, 0, 255}
#define FOUR_COLOR      CLITERAL(Color){0, 0, 128, 255}
#define FIVE_COLOR      CLITERAL(Color){128, 0, 0, 255}
#define SIX_COLOR       CLITERAL(Color){42, 148, 148, 255}
#define SEVEN_COLOR     CLITERAL(Color){0, 0, 0, 255}
#define EIGHT_COLOR     CLITERAL(Color){128, 128, 128, 255}

class Block {
    protected:
        Coordinate _coordinate;
        int _mineCount;
        bool _isHidden;
        bool _isFlagged;

    protected:
        std::vector<Coordinate> getAdjacentCoords(int rowBoundary, int colBoundary);
    
    public:
        Block(int row, int col);
        virtual ~Block();

        virtual bool reveal(std::vector<std::vector<Block*>> &board, int &flagAmount);
        void incrementMineCount();
        void decrementMineCount();
        int getMineCount();
        Color getNumberColor();
        bool isHidden();
        bool isFlagged();
        void toggleFlag(int &flagAmount);
        void resetFromMineBlock(std::vector<std::vector<Block*>> &board);
};

#endif