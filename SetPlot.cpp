#include <SFML/Graphics.hpp>
#include <iostream>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 700;
const int BOARD_SIZE = 500;
const int CELL_SIZE = BOARD_SIZE / 3;

char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
bool isXturn = true;
bool gameOver = false;
int scoreX = 0, scoreO = 0;
sf::Font font;
sf::Text statusText, scoreText, restartText;
sf::RectangleShape restartButton;

void resetGame() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
    isXturn = true;
    gameOver = false;
    statusText.setString("Spieler X ist am Zug");
}

void drawBoard(sf::RenderWindow& window) {
    window.clear(sf::Color::White);

    // Status-Text zeichnen
    window.draw(statusText);
    window.draw(scoreText);

    // Spielfeld zeichnen
    sf::RectangleShape line(sf::Vector2f(BOARD_SIZE, 5));
    line.setFillColor(sf::Color::Black);

    for (int i = 1; i < 3; ++i) {
        line.setPosition(50, i * CELL_SIZE + 150);
        window.draw(line);
        line.setRotation(90);
        line.setPosition(i * CELL_SIZE + 50, 150);
        window.draw(line);
        line.setRotation(0);
    }

    // Symbole zeichnen
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::Black);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] != ' ') {
                text.setString(board[i][j]);
                text.setPosition(j * CELL_SIZE + 80, i * CELL_SIZE + 170);
                window.draw(text);
            }
        }
    }

    // Restart-Button zeichnen
    window.draw(restartButton);
    window.draw(restartText);
}

bool checkWin(char player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

bool isDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return false;
        }
    }
    return true;
}

void handleClick(int x, int y) {
    if (gameOver && x >= 20 && x <= 120 && y >= 20 && y <= 60) {
        resetGame();
        return;
    }
    if (gameOver) return;

    int row = (y - 150) / CELL_SIZE;
    int col = (x - 50) / CELL_SIZE;
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
        board[row][col] = isXturn ? 'X' : 'O';
        if (checkWin(board[row][col])) {
            statusText.setString("Spieler " + std::string(1, board[row][col]) + " gewinnt!");
            gameOver = true;
            if (board[row][col] == 'X') scoreX++;
            else scoreO++;
        }
        else if (isDraw()) {
            statusText.setString("Unentschieden!");
            gameOver = true;
        }
        else {
            isXturn = !isXturn;
            statusText.setString("Spieler " + std::string(1, isXturn ? 'X' : 'O') + " ist am Zug");
        }
        scoreText.setString("X: " + std::to_string(scoreX) + " | O: " + std::to_string(scoreO));
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic Tac Toe");
    if (!font.loadFromFile("fonts/arial.ttf")) return -1;

    // Status-Text
    statusText.setFont(font);
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(50, 60);
    statusText.setString("Spieler X ist am Zug");

    // Punktestand
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(50, 100);
    scoreText.setString("X: 0 | O: 0");

    // Restart-Button
    restartButton.setSize(sf::Vector2f(75, 40));
    restartButton.setFillColor(sf::Color::Red);
    restartButton.setPosition(20, 20);

    restartText.setFont(font);
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(30, 25);
    restartText.setString("Reset");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                handleClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
        drawBoard(window);
        window.display();
    }
    return 0;
}