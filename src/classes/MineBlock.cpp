#include "MineBlock.hpp"

/**
 * @brief Destroy the Mine Block.
 * 
 */
MineBlock::~MineBlock() {}

/**
 * @brief Construct a new Mine Block. Increments the Mine Counter for adjacent non-mine Blocks.
 * 
 * @param board The Board.
 * @param row Board Row.
 * @param col Board Column.
 */
MineBlock::MineBlock(std::vector<std::vector<Block*>> &board, int row, int col) : Block(row, col) {
    _mineCount = MINE_BLOCK;

    for (auto &coord : getAdjacentCoords(board.size(), board[0].size())) {       
        int row = coord.getRow();
        int col = coord.getCol();

        if (board[row][col]->getMineCount() != MINE_BLOCK) 
            board[row][col]->incrementMineCount();    
    }
}

/**
 * @brief Reveal the Block and every other Block on the board.
 * 
 * @param board The game board.
 * @param flagAmount The amount of flags the player has.
 * @return false, meaning the game is over.
 */
bool MineBlock::reveal(std::vector<std::vector<Block*>> &board, int &flagAmount) {
    _isHidden = false;
    
    if (_isFlagged)
        toggleFlag(flagAmount);

    for (auto &line : board)
        for (auto &block : line)
            if (block->isHidden())
                block->reveal(board, flagAmount);
                
    return false;
}