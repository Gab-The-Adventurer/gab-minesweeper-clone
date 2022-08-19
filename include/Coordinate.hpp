#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {
    private:
        int _row;
        int _col;

    public:
        Coordinate(int row = -1, int col = -1);
        int getRow();
        int getCol();
        bool operator == (const Coordinate &rhs);
};

#endif