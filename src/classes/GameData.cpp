#include "GameData.hpp"

/**
 * @brief Delete Blocks from memory and clear board vector.
 * 
 */
void GameData::freeBoard() {
    for (auto &line : _board)
        for (auto &block : line)
            delete block;

    for (auto &line : _board)
        line.clear();
    
    _board.clear();
}

/**
 * @brief Initializes every Block as a normal block.
 * 
 */
void GameData::fillBoard() {
    for (int i = 0; i < _rowAmount; i++) {
        std::vector<Block*> line;

        for (int j = 0; j < _colAmount; j++)
            line.push_back(new Block(i, j));
        
        _board.push_back(line);
    }
}

/**
 * @brief Randomly selects a Block that isn't already a mine 
 * and creates a new Mine Block in its place. 
 * Repeats until it reaches the desired amount of mines.
 * 
 */
void GameData::generateMines() {
    for (int i = 0; i < _mineAmount; i++) {
        int row, col;

        do {
            row = GetRandomValue(0, _rowAmount - 1);
            col = GetRandomValue(0, _colAmount - 1);
        } while (_board[row][col]->getMineCount() == MINE_BLOCK);

        delete _board[row][col];
        _board[row][col] = new MineBlock(_board, row, col);
    }
}

/**
 * @brief Reset general game data and generate a new board.
 * 
 */
void GameData::resetGame() {
    freeBoard();

    _flagAmount = _mineAmount;
    _elapsedTimeInSeconds = 0;
    _isFirstClick = true;
    _gameOver = false;
    _gameOverMine = Coordinate();

    fillBoard();
    generateMines();
}

/**
 * @brief Initializes a new game, generates the board and fills it with mines.
 * 
 * @param rowAmount Board row amount.
 * @param colAmount Board column amount.
 * @param mineAmount Board mine amount.
 */
GameData::GameData(const int &rowAmount, const int &colAmount, const int &mineAmount) {
    _rowAmount = rowAmount;
    _colAmount = colAmount;
    _mineAmount = mineAmount;

    _flagAmount = _mineAmount;
    _elapsedTimeInSeconds = 0;
    _isFirstClick = true;
    _gameOver = false;
    _gameOverMine = Coordinate();

    fillBoard();
    generateMines();
}

/**
 * @brief Destroy the Game Data.
 * 
 */
GameData::~GameData() {
    freeBoard();
}

/**
 * @brief Moves a mine block to a random valid place and resets the old block to a normal one.
 * 
 * @param row Mine Block row.
 * @param col Mine Block column.
 * @return A pointer to the new block in place of the old mine block.
 */
Block* GameData::moveMine(int row, int col) {
    // Reset the mine block to a normal block
    bool wasFlagged = _board[row][col]->isFlagged();

    delete _board[row][col];

    _board[row][col] = new Block(row, col);
    _board[row][col]->resetFromMineBlock(_board);

    if (wasFlagged)
        _flagAmount++;

    // Find a new spot for the mine block that isn't already a mine, isn't the old spot and is hidden.
    int newRow, newCol;

    do {
        newRow = GetRandomValue(0, _rowAmount - 1);
        newCol = GetRandomValue(0, _colAmount - 1);
    } while (_board[newRow][newCol]->getMineCount() == MINE_BLOCK || _board[newRow][newCol]->isHidden() == false || Coordinate(row, col) == Coordinate(newRow, newCol));

    wasFlagged = _board[newRow][newCol]->isFlagged();

    delete _board[newRow][newCol];
    _board[newRow][newCol] = new MineBlock(_board, newRow, newCol);

    // Preserves flag on the new mine block (if the old one was flagged)
    if (wasFlagged) {
        _flagAmount++;
        _board[newRow][newCol]->toggleFlag(_flagAmount);
    }

    return _board[row][col];
}

/**
 * @brief Returns the board vector.
 * 
 */
std::vector<std::vector<Block*>>& GameData::getBoard() {
    return _board;
}

/**
 * @brief Returns the amount of rows.
 * 
 */
int GameData::getRows() {
    return _rowAmount;
}

/**
 * @brief Returns the amount of columns.
 * 
 */
int GameData::getCols() {
    return _colAmount;
}

/**
 * @brief Returns the amount of mines.
 * 
 */
int GameData::getMines() {
    return _mineAmount;
}

/**
 * @brief Returns the amount of flags.
 * 
 */
int& GameData::getFlags() {
    return _flagAmount;
}

/**
 * @brief Returns true if the game is over, false otherwise.
 * 
 */
bool GameData::isGameOver() {
    return _gameOver;
}

/**
 * @brief Sets the game as over and saves which mine caused it.
 * Defaults to (-1, -1), meaning the game is over by winning and not by revealing a mine.
 * 
 * @param row Game Over mine row.
 * @param col Game Over mine column.
 */
void GameData::setGameOver(int row, int col) {
    _gameOver = true;

    _gameOverMine = Coordinate(row, col);
}

/**
 * @brief Returns the Game Over mine coordinate.
 * 
 */
Coordinate GameData::getGameOverMine() {
    return _gameOverMine;
}

/**
 * @brief Returns true if the first click is active, false otherwise.
 * 
 */
bool GameData::isFirstClick() {
    return _isFirstClick;
}

/**
 * @brief Sets first click as used.
 * 
 */
void GameData::setFirstClickUsed() {
    _isFirstClick = false;
}

/**
 * @brief Adds a second to the total elapsed time.
 * 
 */
void GameData::addSecond() {
    _elapsedTimeInSeconds++;
}

/**
 * @brief Returns total elapsed time in the format of HH:MM:SS.
 * 
 */
std::string GameData::getElapsedTime() {
    uint64_t minutes = _elapsedTimeInSeconds / 60;
    uint64_t hours = minutes / 60;
    uint64_t seconds = _elapsedTimeInSeconds - 60 * minutes;
    minutes -= 60 * hours;

    std::ostringstream time;

    time << std::setw(2) << std::setfill('0') << hours << ":";
    time << std::setw(2) << std::setfill('0') << minutes << ":";
    time << std::setw(2) << std::setfill('0') << seconds;

    return time.str();
}