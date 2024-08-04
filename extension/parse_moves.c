#include "parse_moves.h"

bool isJumpPossible(GameState *game, int x, int y)
{
    int valid1 = isValid(game, x, y, x - 2, y - 2);
    int valid2 = isValid(game, x, y, x - 2, y + 2);
    int valid3 = isValid(game, x, y, x + 2, y - 2);
    int valid4 = isValid(game, x, y, x + 2, y + 2);

    switch (game->board[x][y])
    {
    case RED:
        return (valid1 || valid2);
        break;
    case BLACK:
        return (valid3 || valid4);
        break;
    case RED_KING:
        return (valid1 || valid2 || valid3 || valid4);
        break;
    case BLACK_KING:
        return (valid1 || valid2 || valid3 || valid4);
        break;
    default:
        return 0;
    }
}
bool isExit(char *input) {
    for(int i=0;i<strlen(input);i++) {
        input[i] = tolower(input[i]);
    }
    return (!strcmp("exit", input));
}

bool isHelp(char *input) {
    for(int i=0;i<strlen(input);i++) {
        input[i] = tolower(input[i]);
    }
    return (!strcmp("help", input));
}

int main(void) {
    printf("Welcome to Checkers.\n");
    GameState game;
    initializeBoard(&game);
    char *all_moves[21] = {"f1e2", "c2d1", "f3e4", "b3c2", "e4d5", "c2d3", "g2f3", "c6e4", "e4g2", "h1f3", "d3f1", "f3e2", "c4d3", "e2c4", "a4b3", "f5e6", "b5c6", "c4b5", "b1c2", "b5a4", "c2d3"};
    // processMoves(&game, all_moves, 22);
    // initialiseBlackWin(&game);
    printBoard(&game);

    char jump_to[3];
    jump_to[2] = '\0';
    bool jump_chk = false;

    int i= 0;

    while (isEndGameWinner(&game) == NO_TERM)
    {
        // input 4 characters

        char input[10];
        printf("input next move. Enter \"exit\" to exit. Enter \"help\" for instructions.\n");
        if (i < 21) {
            strcpy(input, all_moves[i]);
            i++;
        }
        else {
            scanf("%s", input);
        }
        // scanf("%s", input);
        

        if(isExit(input)) {
            printf("Exited game.\n");
            return 0;
        }

        if(isHelp(input)) {
            printf("Checkers Game Instructions:\n1. The game is played on an 8x8 board.\n2. Each player starts with 12 pieces placed on alternate squares of the three rows closest to them.\n3. Players take turns moving one piece at a time.\n4. Regular pieces can only move diagonally forward.\n5. To capture an opponent's piece, you must jump over it to an empty square directly beyond it.\n6. If a piece reaches the far side of the board, it becomes a King.\n7. Kings can move diagonally both forward and backward.\n8. The game ends when one player cannot make a move, either because all their pieces are captured or blocked.\n9. The player with remaining pieces or the player who blocks the opponent wins.\n10. To move a piece enter the coordinate of the piece you wish to move followed by the coordinate of the place you wish to move the piece to, eg. f1e2 moves the piece at f1 to e2\n");
            return 0;
        }

        if (!validInput(input))
        {
            continue;
        }

        // printf("jump chk %d\n", jump_chk);
            if (jump_chk) {
                if(toupper(input[0]) - 'A' != jump_to[0] ||  input[1] - '1' != jump_to[1]) {
                    printf("Please input jump move using same piece. \n");
                    continue;
                }
            }

       // printf("%d %d %d %d\n", toupper(input[0]) - 'A', input[1] - '1', toupper(input[2]) - 'A', input[3] - '1');

        int mp = movePiece(&game, toupper(input[0]) - 'A', input[1] - '1', toupper(input[2]) - 'A', input[3] - '1');

        if (abs((input[1] - '1') - (input[3] - '1')) == 2)
        {
            game.currentPlayer = !game.currentPlayer;
            
            isValid(&game, toupper(input[0]) - 'A', input[1] - '1', toupper(input[2]) - 'A', input[3] - '1');
          
            if (mp)
            {
                // game.currentPlayer = !game.currentPlayer;
                if (isJumpPossible(&game, toupper(input[2]) - 'A', input[3] - '1'))
                {
                    jump_chk = true;
                    // So if jump chk true then u need to check the next input = curr output
                    jump_to[0] = toupper(input[2]) - 'A';
                    jump_to[1] = input[3] - '1';
                }
                else
                {
                    jump_chk = false;
                    game.currentPlayer = !game.currentPlayer;
                }
            } else {
                // check
                game.currentPlayer = !game.currentPlayer;
            }
        }
        
        makeKing(&game);
        printBoard(&game);
    }

    return 0;
}

int validInput(char *input)
{
    if (strlen(input) != 4 || !isalpha(input[0]) || !isalpha(input[2]) || !isdigit(input[1]) || !isdigit(input[3]))
    {
        printf("invalid input: please enter in format\n(start x co-ordinate, start y co-ordinate, end x co-ordinate, end x co-ordinate)");
        printf("<(a-h)><(0-7)><(a-h)><(0-7)> e.g. a0b1");
        return 0;
    }
    return 1;
}