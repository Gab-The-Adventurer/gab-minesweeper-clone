#include "Coordinate.hpp"

/**
 * @brief Construct a new Coordinate. Defaults to (-1, -1).
 * 
 * @param row Row.
 * @param col Column.
 */
Coordinate::Coordinate(int row, int col) {
    _row = row;
    _col = col;
}

/**
 * @brief Returns the coordinate row.
 * 
 */
int Coordinate::getRow() {
    return _row;
}

/**
 * @brief Returns the coordinate column.
 * 
 */
int Coordinate::getCol() {
    return _col;
}  

/**
 * @brief Compares two coordinates.
 * 
 * @param rhs Coordinate to compare to.
 * @return true if both are equal, false otherwise.
 */
bool Coordinate::operator == (const Coordinate &rhs) {
    return this->_row == rhs._row && this->_col == rhs._col;
}