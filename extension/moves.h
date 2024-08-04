#ifndef MOVES_H
#define MOVES_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
extern char buffer[1000];
// checks if a move is valid
extern int isValid(GameState *game, int startX, int startY, int endX, int endY);

extern int isValidJump(GameState *game, int startX, int startY, int endX, int endY);
extern int movePiece(GameState *game, int startX, int startY, int endX, int endY);

// checks if it is the end of a game (call this after every move?)
extern int isValidKingMove(GameState *game, int startX, int startY, int endX, int endY);

#endif
