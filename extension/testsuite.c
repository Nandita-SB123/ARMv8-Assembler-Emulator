#include "testsuite.h"

void testInitializeBoard() {
    GameState game;
    initializeBoard(&game);

    // Check if all pieces are correctly placed
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i < 3 && (i + j) % 2 != 0) {
                assert(game.board[i][j] == BLACK);
            } else if (i > 4 && (i + j) % 2 != 0) {
                assert(game.board[i][j] == RED);
            } else {
                assert(game.board[i][j] == EMPTY);
            }
        }
    }
    printf("testInitializeBoard passed.\n");
}

void testIsValidMove() {
    GameState game;
    initializeBoard(&game);

    // Invalid move general
    assert(isValid(&game, 10, 9, -1, 1) == 1); // Out of bounds

    // Valid moves for RED
    assert(isValid(&game, 5, 0, 4, 1) == 1); // Simple forward move
    assert(isValid(&game, 5, 2, 4, 3) == 1); // Simple forward move

    // Invalid moves for RED
    assert(isValid(&game, 5, 0, 3, 2) == 0); // Too far
    assert(isValid(&game, 5, 0, 5, 1) == 0); // Same row

    // Valid moves for BLACK
    game.currentPlayer = 1;
    assert(isValid(&game, 2, 1, 3, 2) == 1); // Simple forward move
    assert(isValid(&game, 2, 3, 3, 4) == 1); // Simple forward move

    // Invalid moves for BLACK
    assert(isValid(&game, 2, 1, 4, 3) == 0); // Too far
    assert(isValid(&game, 2, 1, 2, 2) == 0); // Same row

    printf("testIsValidMove passed.\n");
}

void testMakeMove() {
    GameState game;
    initializeBoard(&game);

    // Make a valid move for RED
    assert(movePiece(&game, 5, 0, 4, 1) == 1);
    assert(game.board[5][0] == EMPTY);
    assert(game.board[4][1] == RED);

    // Make an invalid move for RED
    assert(movePiece(&game, 5, 2, 3, 4) == 0);

    // Make a valid move for BLACK
    game.currentPlayer = 1;
    assert(movePiece(&game, 2, 1, 3, 2) == 1);
    assert(game.board[2][1] == EMPTY);
    assert(game.board[3][2] == BLACK);

    printf("testMakeMove passed.\n");
}

void testCapturePiece() {
    GameState game;
    initializeBoard(&game);

    // Set up a capture scenario
    game.board[4][3] = BLACK;
    assert(movePiece(&game, 5, 2, 3, 4) == 1); // RED captures BLACK

    assert(game.board[5][2] == EMPTY);
    assert(game.board[4][3] == EMPTY);
    assert(game.board[3][4] == RED);
    assert(game.redCaptured == 1);

    printf("testCapturePiece passed.\n");
}

void testKingPiece() {
    GameState game;
    initializeBoard(&game);

    // Move a RED piece to the last row
    game.board[1][6] = RED;
    game.board[0][7] = EMPTY;
    makeKing(&game);

    assert(game.board[0][7] == RED_KING);

    // Move a BLACK piece to the first row
    game.board[6][1] = BLACK;
    game.board[7][0] = EMPTY;
    makeKing(&game); 
    }

void testIsExit() {
    assert(isExit("exit") == true);
    assert(isExit("Exit") == true);
    assert(isExit("EXIT") == true);
    assert(isExit("exIt") == true);
    assert(isExit("exitt") == false);

    printf("testIsExit passed.\n");
}

#define RUN_TESTS
#ifdef RUN_TESTS

int main(void) {
    printf("Running tests for Checkers game...\n");

    // Run each test case
    testInitializeBoard();
    testIsValidMove();
    testMakeMove();
    testCapturePiece();
    testKingPiece();
    testIsExit();

    printf("All tests passed successfully!\n"); }

#endif




