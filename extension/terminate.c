#include "terminate.h"
Result isEndGameWinner(GameState *game) {
    
    // game over conditions
    if (game -> blackCaptured == START_PIECES) {
        printf("RED wins\n");
        return RED_WINS;
    }
    else if (game -> redCaptured == START_PIECES) {
        printf("BLACK wins\n");
        return BLACK_WINS;
    }

    // no valid move or jump from normal pieces.

    bool b_no_chk = true; // true if there's no valid moves of player left

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            // first check if valid move
            if(game -> board[i][j] == BLACK) {
                for(int k = i-2; k < i+2; k++) {
                    for(int l = j-2; l < j+2; l++) {
                        if(isValid(game, i, j, k, l) || isValidJump(game, i, j, k, l)) {
                            b_no_chk = false;
                            break;
                        }
                    }
                }
            }

            if (game -> board[i][j] == BLACK_KING) {
                for(int k = i-2; k < i+2; k++) {
                    for(int l = j-2; l < j+2; l++) {
                        if(isValidKingMove(game, i, j, k, l)) {
                            b_no_chk = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    bool r_no_chk = true; // true if there's no valid moves of player left

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if (game -> board[i][j] == RED) {
                for(int k = i-2; k < i+2; k++) {
                    for(int l = j-2; l < j+2; l++) {
                        if(isValid(game, i, j, k, l) || isValidJump(game, i, j, k, l)) {
                            r_no_chk = false;
                            break;
                        }
                    }
                }
            }
            
            if (game -> board[i][j] == RED_KING) {
                for(int k = i-2; k < i+2; k++) {
                    for(int l = j-2; l < j+2; l++) {
                        if(isValidKingMove(game, i, j, k, l)) {
                            r_no_chk = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    // draw condition: both don't have valid moves left.
    // will return DRAW in this case.

    if (b_no_chk && r_no_chk) {
        printf("DRAW\n");
        return DRAW;
    }
    else if (b_no_chk) {
        printf("RED wins\n");
        return RED_WINS;
    }
    else if (r_no_chk) {
        printf("BLACK wins\n");
        return BLACK_WINS;
    }

    return NO_TERM;
}


void makeKing(GameState *game) {
    // black
    for(int i=0;i<SIZE;i+=2) {
        if (game -> board[SIZE-1][i] == BLACK) {
            game -> board[SIZE-1][i] = BLACK_KING;
        }
    }
    // red
    for(int i=1;i<SIZE;i+=2) {
        if (game -> board[0][i] == RED) {
            game -> board[0][i] = RED_KING;
        }
    }
}