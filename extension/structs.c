#include "structs.h"


// global variable to track if game ends
bool gameEnd = false;


void initializeBoard(GameState *game) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i < 3 && (i + j) % 2 != 0) {
                game->board[i][j] = BLACK;
            } else if (i > 4 && (i + j) % 2 != 0) {
                game->board[i][j] = RED;
            } else {
                game->board[i][j] = EMPTY;
            }
        }
    }
    game->currentPlayer = 0; // RED starts
    game->redCaptured = 0;
    game->blackCaptured = 0;
}

void initialiseBlackWin(GameState *game) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            game->board[i][j] = EMPTY;
        }
    }
    game->board[2][1] = BLACK;
    game->board[3][2] = RED;
    game->currentPlayer = 1; // RED starts
    game->redCaptured = 11;
    game->blackCaptured = 11;
}