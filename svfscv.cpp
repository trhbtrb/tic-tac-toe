#include <iostream>
#include <limits>
using namespace std;

char board[3][3] = { {'1', '2', '3'},
                     {'4', '5', '6'},
                     {'7', '8', '9'} };
char playerMarker, aiMarker;
int currentPlayer;

void drawBoard() {
    cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
    cout << "---|---|---" << endl;
    cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
    cout << "---|---|---" << endl;
    cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
}

bool placeMarker(int slot, char marker) {
    int row = (slot - 1) / 3;
    int col = (slot - 1) % 3;
    if (board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = marker;
        return true;
    } else {
        return false;
    }
}

int checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return (board[i][0] == playerMarker) ? 1 : -1;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return (board[0][i] == playerMarker) ? 1 : -1;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return (board[0][0] == playerMarker) ? 1 : -1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return (board[0][2] == playerMarker) ? 1 : -1;
    }
    return 0;
}

bool isBoardFull() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') return false;
        }
    }
    return true;
}

int minimax(bool isAI) {
    int score = checkWinner();
    if (score == 1) return -10;
    if (score == -1) return 10;
    if (isBoardFull()) return 0;

    int bestScore = isAI ? -std::numeric_limits<int>::max() : std::numeric_limits<int>::max();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                board[i][j] = isAI ? aiMarker : playerMarker;
                int currentScore = minimax(!isAI);
                board[i][j] = '1' + 3 * i + j;
                bestScore = isAI ? max(bestScore, currentScore) : min(bestScore, currentScore);
            }
        }
    }
    return bestScore;
}

int findBestMove() {
    int bestScore = -std::numeric_limits<int>::max();
    int move = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                board[i][j] = aiMarker;
                int score = minimax(false);
                board[i][j] = '1' + 3 * i + j;

                if (score > bestScore) {
                    bestScore = score;
                    move = 3 * i + j + 1;
                }
            }
        }
    }
    return move;
}

void switchPlayer() {
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

void game() {
    cout << "Choose your marker (X or O): ";
    cin >> playerMarker;
    aiMarker = (playerMarker == 'X') ? 'O' : 'X';

    currentPlayer = 1;  // Player starts first

    drawBoard();
    while (true) {
        int winner = checkWinner();
        if (winner != 0 || isBoardFull()) {
            if (winner == 1) cout << "Player wins!\n";
            else if (winner == -1) cout << "AI wins!\n";
            else cout << "It's a tie!\n";
            break;
        }

        if (currentPlayer == 1) {
            int slot;
            cout << "Your turn. Enter your slot (1-9): ";
            cin >> slot;

            if (slot < 1 || slot > 9 || !placeMarker(slot, playerMarker)) {
                cout << "Invalid move. Try again.\n";
                continue;
            }
        } else {
            int bestMove = findBestMove();
            placeMarker(bestMove, aiMarker);
            cout << "AI chose slot " << bestMove << endl;
        }

        drawBoard();
        switchPlayer();
    }
}

int main() {
    game();
    return 0;
}
