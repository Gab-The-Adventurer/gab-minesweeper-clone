#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <iostream>
#include <sstream>
#include <iomanip>

#include "raylib.h"
#include "MineBlock.hpp"

class GameData {
    private: 
        std::vector<std::vector<Block*>> _board;
        int _rowAmount;
        int _colAmount;
        int _mineAmount;
        int _flagAmount;
        bool _isFirstClick;
        bool _gameOver;
        Coordinate _gameOverMine;
        uint64_t _elapsedTimeInSeconds;
        
    private:
        void fillBoard();
        void freeBoard();
        void generateMines();

    public:
        ~GameData();
        GameData(const int &rowAmount, const int &colAmount, const int &mineAmount);

        std::vector<std::vector<Block*>>& getBoard();
        int getRows();
        int getCols();
        int getMines();
        int& getFlags();

        bool isFirstClick();
        void setFirstClickUsed();
        Block* moveMine(int row, int col);

        bool isGameOver();
        void setGameOver(int row = -1, int col = -1);
        void resetGame();
        Coordinate getGameOverMine();

        void addSecond();
        std::string getElapsedTime();
};

#endif