#ifndef ARRAY_HASHER
#define ARRAY_HASHER

#include "boardStateStruct.hpp"

struct BoardHasher{
    std::size_t operator()(struct boardState const bState) const{
        size_t s = 0x3a7eb429; 
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                s = (s >> 1) | (s << (sizeof(size_t) * 8 - 1));
                s ^= bState.board[i][j] * 0xee6b2807;
            }
        }
        s *= 0xee6b2807;
        s ^= s >> 16;
        return s;
    }
};


#endif