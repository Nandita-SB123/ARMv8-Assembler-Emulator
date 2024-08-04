#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>
#include <stdbool.h>
#define SIZE 8
#define START_PIECES 12

typedef enum { EMPTY, RED, BLACK, RED_KING, BLACK_KING } Piece;
typedef enum {NO_TERM, RED_WINS, BLACK_WINS, DRAW} Result;
typedef struct{
    Piece board[SIZE][SIZE];
    int currentPlayer; // 0 for RED, 1 for BLACK
    int redCaptured;
    int blackCaptured;
} GameState;

extern void initializeBoard(GameState *game);
extern Result isEndGameWinner(GameState *game);
extern void makeKing(GameState *game);

extern void initialiseBlackWin(GameState *game);

#endif
