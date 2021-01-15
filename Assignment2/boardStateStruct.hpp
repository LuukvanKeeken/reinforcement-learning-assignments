#ifndef BOARD_STATE
#define BOARD_STATE

/* Struct for representing a board state. */
struct boardState{
    char board[3][3];
    bool operator==(const boardState& otherBoard) const {
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] != otherBoard.board[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
};

#endif