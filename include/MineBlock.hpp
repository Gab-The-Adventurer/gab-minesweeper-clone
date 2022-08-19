#ifndef MINE_BLOCK_H
#define MINE_BLOCK_H

#include "Block.hpp"

class MineBlock : public Block {
    public:
        MineBlock(std::vector<std::vector<Block*>> &board, int row, int col);
        virtual bool reveal(std::vector<std::vector<Block*>> &board, int &flagAmount) override;
        virtual ~MineBlock();
};

#endif