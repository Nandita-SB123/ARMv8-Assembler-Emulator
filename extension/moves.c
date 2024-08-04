#include "moves.h"
#include <stdbool.h>

// this is a global variable we use to track what piece we are moving
Piece current_piece;
int validJump;
int validMove;

char buffer[1000];

int isValid(GameState *game, int startX, int startY, int endX, int endY)
{
    // check that the inputted point is within the boundaries of the board
    int diffX = startX - endX;
    int diffY = startY - endY;

    if (startX < 0 || startX > SIZE || startY < 0 || startY > SIZE || endX < 0 || endX > SIZE || endY < 0 || endY > SIZE)
    {
        sprintf(buffer, "This position is not valid on the board\n");
        return 0;
    }
    else if (!(abs(diffX) == 1 || abs(diffX) == 2) || !(abs(diffY) == 1 || abs(diffY) == 2))
    {
        sprintf(buffer, "Cannot move this far\n");
        return 0;
    }
    // check that the square they are moving to isn't already occupied
    else if (game->board[endX][endY] != EMPTY)
    {
        sprintf(buffer, "The target square you are trying to go to is not EMPTY\n");
        return 0;
    }
    // it checks that the piece in the square is that of the same colour
    else if (!(game->board[startX][startY] == RED || game->board[startX][startY] == RED_KING) && game->currentPlayer == 0)
    {
        sprintf(buffer, "This player does not have a piece on this square\n");
        return 0;
    }
    else if (!(game->board[startX][startY] == BLACK || game->board[startX][startY] == BLACK_KING) && game->currentPlayer == 1)
    {
        sprintf(buffer, "This player does not have a piece on this square\n");
        return 0;
    }
    else if (!(abs(diffX) == abs(diffY)))
    {
        sprintf(buffer, "Move not diagonal\n");
        return 0;
    }
    int validKing = 1;
    switch (game->board[startX][startY])
    {
    case RED:
        if (diffX < 0)
        {

            sprintf(buffer, "red can only move upwards, not downwards\n");
            return 0;
        }
        break;
    case BLACK:
        if (diffX > 0)
        {
            sprintf(buffer, "black can only move downwards, not upwards\n");
            return 0;
        }
        break;
    case RED_KING:
        sprintf(buffer, "\n\nAbout to check valid king move\n\n");
        validKing = isValidKingMove(game, startX, startY, endX, endY);
        return validKing;
        break;
    case BLACK_KING:
        validKing = isValidKingMove(game, startX, startY, endX, endY);
        return validKing;
        break;
    default:
        return 1;
    }

    // printf("reached before jump\n");

    // END THIS IF ELSE
    if (abs(diffX) == 2)
    {
        // printf("above calling valid jump %d %d %d %d     %d\n", startX, startY, endX, endY, game->board[(startX + endX) / 2][(startY + endY) / 2]);
        validJump = isValidJump(game, startX, startY, endX, endY);
        return validJump;
    }
    return 1;
}

int isValidJump(GameState *game, int startX, int startY, int endX, int endY)
{

    // you cannot jump over a piece of the same colour as yours
    if (game->currentPlayer == 1 && !(game->board[(startX + endX) / 2][(startY + endY) / 2] == RED || game->board[(startX + endX) / 2][(startY + endY) / 2] == RED_KING))
    {
        sprintf(buffer, "in if cond You cannot jump over one of your own pieces\n");
        return 0;
    }
    else if (game->currentPlayer == 0 && !(game->board[(startX + endX) / 2][(startY + endY) / 2] == BLACK || game->board[(startX + endX) / 2][(startY + endY) / 2] == BLACK_KING))
    {
        // printf("inside valid jump else if\n");
        sprintf(buffer, "in else if cond You cannot jump over one of your own pieces. %d %d %d %d     %d\n", startX, startY, endX, endY, game->board[(startX + endX) / 2][(startY + endY) / 2]);

        return 0;
    }
    return 1;
}

// this just moves a piece, it doesnt deal with capturing a piece or kinging logic
int movePiece(GameState *game, int startX, int startY, int endX, int endY)
{
    current_piece = game->board[startX][startY];
    if (isValid(game, startX, startY, endX, endY))
    {
        game->board[startX][startY] = EMPTY;
        game->board[endX][endY] = current_piece;

        if (abs(startX - endX) == 2)
        {
            game->board[(startX + endX) / 2][(startY + endY) / 2] = EMPTY;
            if (game->currentPlayer == 0)
            {
                game->blackCaptured++;
            }
            else
            {
                game->redCaptured++;
            }
        }
        game->currentPlayer = !game->currentPlayer;
    }
    else
    {
        printf("Invalid move\n");
        printf("%s\n", buffer);
        return 0;
    }

    return 1;
}

int isValidKingMove(GameState *game, int x1, int y1, int x2, int y2)
{
    // starting piece should be a king
    if (!(game->board[x1][y1] == RED_KING || game->board[x1][y1] == BLACK_KING))
    {
        return 0;
    }
    // game->currentPlayer = !game->currentPlayer;
    // // any jump or move is valid king move
    // if (isValid(game, x1, y1, x2, y2) || isValidJump(game, x1, y1, x2, y2)) {
    //     return true;
    // }

    // // now check valid or valid jump but backwards
    // if (isValid(game, x2, y2, x1, y1) || isValidJump(game, x2, y2, x1, y1)) {
    //     return true;
    // }

    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dx) == 1 && abs(dy) == 1)
    {
        // game->currentPlayer = !game->currentPlayer;
        return game->board[x2][y2] == EMPTY; // Ensure the destination is empty
    }

    if (abs(dx) == 2 && abs(dy) == 2)
    {
        sprintf(buffer, "dx: %d, dy: %d", dx, dy);
        int mx = x1 + dx / 2;
        int my = y1 + dy / 2;
        if (game->board[x2][y2] == EMPTY &&
            ((game->board[x1][y1] == RED_KING && (game->board[mx][my] == BLACK || game->board[mx][my] == BLACK_KING)) || (game->board[x1][y1] == BLACK_KING && (game->board[mx][my] == RED || game->board[mx][my] == RED_KING))))
        {
            // game->currentPlayer = !game->currentPlayer;
            return 1;
        }
    }
    // game->currentPlayer = !game->currentPlayer;
    return 0;
}

// returns the winner, if terminated. Else returns no_term.

// // have x1 =, x2 =, y1 =, y2 =.
// // for multiple jumps:
// while (isValidJump(game, x1, y1, x2, y2)) {
//     // do the jumping
//     // then input again x1 =, x2 =, y1 =, y2 =.
// }
