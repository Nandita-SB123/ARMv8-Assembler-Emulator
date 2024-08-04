#include "main.h"

char letters[SIZE + 1] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', '\0'};

// prints the current version of the board
void printBoard(GameState *game)
{
    // Print the column numbers at the top
    printf("    ");
    for (int j = 1; j <= SIZE; j++)
    {
        printf("%d ", j);
    }
    printf("\n    ");
    for (int j = 0; j < (SIZE * 2) - 1; j++)
    {
        printf("-");
    }
    printf("\n");

    // Print the board with row letters on the left
    for (int i = 0; i < SIZE; ++i)
    {
        // Print the row letter
        printf("%c ", letters[i]);
        printf("| ");

        for (int j = 0; j < SIZE; ++j)
        {
            switch (game->board[i][j])
            {
            case EMPTY:
                printf(". ");
                break;
            case RED:
                printf("r ");
                break;
            case BLACK:
                printf("b ");
                break;
            case RED_KING:
                printf("R ");
                break;
            case BLACK_KING:
                printf("B ");
                break;
            }
        }
        printf("\n");
    }
    printf("Current Player: %s\n", game->currentPlayer == 0 ? "RED" : "BLACK");
}