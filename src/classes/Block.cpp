#include "Block.hpp"

/**
 * @brief Construct a new Block.
 * 
 * @param row Board Row.
 * @param col Board Column.
 */
Block::Block(int row, int col) {
    _coordinate = Coordinate(row, col);
    _mineCount = 0;
    _isHidden = true;
    _isFlagged = false;
}

/**
 * @brief Destroy the Block.
 * 
 */
Block::~Block() {}

/**
 * @brief Get every adjacent coordinate to the block.
 * 
 * @param rowBoundary Board row size.
 * @param colBoundary Board column size.
 * @return A vector with each coordinate.
 */
std::vector<Coordinate> Block::getAdjacentCoords(int rowBoundary, int colBoundary) {
    std::vector<Coordinate> adjacent;

    int row = _coordinate.getRow();
    int col = _coordinate.getCol();

    for (int i = row - 1; i <= row + 1; i++) {     
        if (i < 0 || i > rowBoundary - 1)      
            continue;  
              
        for (int j = col - 1; j <= col + 1; j++) {
            if (j < 0 || j > colBoundary - 1)   
                continue;    

            if (i == row && j == col)     // Does not add its own coordinate
                continue;

            adjacent.push_back(Coordinate(i, j));
        }
    }

    return adjacent;
}

/**
 * @brief Toggles the flag ON and OFF
 * 
 */
void Block::toggleFlag(int &flagAmount) {
    _isFlagged = !_isFlagged;

    if (_isFlagged)
        flagAmount--;
    else
        flagAmount++;
}

/**
 * @brief Reveal the block and every non-mine block adjacent to it, causing a chain reaction.
 * If it has mines adjacent to it, only reveals itself.
 * 
 * @param board The game board.
 * @param flagAmount The amount of flags the player has.
 * @return true, meaning the game can continue.
 */
bool Block::reveal(std::vector<std::vector<Block*>> &board, int &flagAmount) {
    _isHidden = false;
    
    if (_isFlagged)
        toggleFlag(flagAmount); 

    // For every adjacent non-mine block
    if (_mineCount == 0) {
        for (auto &coord : getAdjacentCoords(board.size(), board[0].size())) {       
            int row = coord.getRow();
            int col = coord.getCol();

            // If it's hidden, reveals it (this check is important to avoid infinite loops)
            if (board[row][col]->isHidden()) 
                board[row][col]->reveal(board, flagAmount);     
        }
    }

    return true;
}

/**
 * @brief Resets the block state from a mine block previously in its place.
 * 
 * @param board The game board.
 */
void Block::resetFromMineBlock(std::vector<std::vector<Block*>> &board) {
    for (auto &coord : getAdjacentCoords(board.size(), board[0].size())) {       
        int row = coord.getRow();
        int col = coord.getCol();

        if (board[row][col]->getMineCount() != MINE_BLOCK) 
            board[row][col]->decrementMineCount();
        else
            this->incrementMineCount(); 
    }
}

/**
 * @brief Returns the appropriate number color based on the block's mine count.
 * 
 */
Color Block::getNumberColor() {
    switch (_mineCount) {
    case 1:
        return ONE_COLOR;
    
    case 2:
        return TWO_COLOR;

    case 3:
        return THREE_COLOR;

    case 4:
        return FOUR_COLOR;

    case 5:
        return FIVE_COLOR;

    case 6:
        return SIX_COLOR;

    case 7:
        return SEVEN_COLOR;

    case 8:
        return EIGHT_COLOR;

    default:
        return WHITE;
    }
}

/**
 * @brief Get the amount of mines surrounding the Block.
 * 
 * @return The amount of mines. Returns -1 if the Block is a mine.
 */
int Block::getMineCount() {
    return _mineCount;
}

/**
 * @return true if the Block is hidden, false otherwise.
 */
bool Block::isHidden() {
    return _isHidden;
}

/**
 * @return true if the Block is flagged, false otherwise.
 */
bool Block::isFlagged() {
    return _isFlagged;
}

/**
 * @brief Increment Mine Count by one.
 * 
 */
void Block::incrementMineCount() {
    _mineCount++;
}

/**
 * @brief Decrement Mine Count by one.
 * 
 */
void Block::decrementMineCount() {
    _mineCount--;
}