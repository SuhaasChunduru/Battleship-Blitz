#include <iostream>
#include <vector>
#include <string>

// Constants
const int BOARD_SIZE = 10;
const int MAX_ATTEMPTS = 5; // Maximum attacks per player

// Coordinate Class
class Coordinate {
public:
    int x, y;
    Coordinate(int x, int y) : x(x), y(y) {}
};

// Ship Class
class Ship {
public:
    std::string name;
    int size;
    std::vector<Coordinate> coordinates;

    Ship(std::string name, int size) : name(name), size(size) {}

    void setPosition(const std::vector<Coordinate>& coords) {
        coordinates = coords;
    }
};

// Board Class
class Board {
private:
    std::vector<std::vector<char>> grid;
    std::vector<Ship> ships;

public:
    Board() : grid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '-')) {}

    // Display board
    void display(bool showShips = false) {
        std::cout << "  ";
        for (int i = 0; i < BOARD_SIZE; i++) std::cout << i << " ";
        std::cout << std::endl;

        for (int i = 0; i < BOARD_SIZE; i++) {
            std::cout << i << " ";
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (showShips || grid[i][j] == 'X' || grid[i][j] == 'M')
                    std::cout << grid[i][j] << " ";
                else
                    std::cout << "- ";
            }
            std::cout << std::endl;
        }
    }

    // Place ship on the board
    bool placeShip(Ship& ship, int startX, int startY, char direction) {
        std::vector<Coordinate> coords;

        // Check if the ship can be placed
        for (int i = 0; i < ship.size; i++) {
            int x = startX + (direction == 'H' ? 0 : i);
            int y = startY + (direction == 'H' ? i : 0);

            // Check bounds and if cell is occupied
            if (x >= BOARD_SIZE || y >= BOARD_SIZE || grid[x][y] != '-') {
                std::cout << "Invalid placement for " << ship.name << " at (" << startX << "," << startY << ")\n";
                return false;
            }
            coords.emplace_back(x, y);
        }

        // Place the ship
        for (const auto& coord : coords) {
            grid[coord.x][coord.y] = 'S';
        }
        ship.setPosition(coords);
        ships.push_back(ship); // Store ship in board's ship list

        return true;
    }

    // Attack position
    bool attack(int x, int y) {
        if (grid[x][y] == 'S') {
            grid[x][y] = 'X'; // Mark as hit
            std::cout << "Hit!\n";
            return true;
        } else if (grid[x][y] == '-') {
            grid[x][y] = 'M'; // Mark as miss
            std::cout << "Miss!\n";
            return false;
        } else {
            std::cout << "This position has already been attacked!\n";
            return false;
        }
    }

    // Check if all ships are sunk
    bool allShipsSunk() {
        for (const auto& ship : ships) {
            for (const auto& coord : ship.coordinates) {
                if (grid[coord.x][coord.y] != 'X') {
                    return false;
                }
            }
        }
        return true;
    }
};

// Player Class
class Player {
private:
    Board board;

public:
    std::string name;
    int hits = 0;  // Track successful hits for comparison at game end

    Player(std::string name) : name(name) {}

    void placeShips() {
        Ship carrier("Carrier", 5), battleship("Battleship", 4), cruiser("Cruiser", 3);
        Ship submarine("Submarine", 3), destroyer("Destroyer", 2);
        std::vector<Ship*> ships = {&carrier, &battleship, &cruiser, &submarine, &destroyer};

        for (auto& ship : ships) {
            board.display(true);
            std::cout << name << ", place your " << ship->name << " of size " << ship->size << std::endl;

            bool placed = false;
            while (!placed) {
                int x, y;
                char direction;
                std::cout << "Enter start position (x y) and direction (H for horizontal, V for vertical): ";
                std::cin >> x >> y >> direction;
                direction = toupper(direction);

                if (direction == 'H' || direction == 'V') {
                    placed = board.placeShip(*ship, x, y, direction);
                } else {
                    std::cout << "Invalid direction. Use 'H' for horizontal or 'V' for vertical.\n";
                }
            }
        }
    }

    bool makeMove(Player& opponent) {
        board.display();
        int x, y;
        std::cout << name << ", enter attack coordinates (x y): ";
        std::cin >> x >> y;

        bool hit = opponent.board.attack(x, y);
        if (hit) {
            hits++;
        }
        return hit;
    }

    bool hasLost() {
        return board.allShipsSunk();
    }
};

// Game Class
class Game {
private:
    Player player1;
    Player player2;
    int player1Attempts = 0;
    int player2Attempts = 0;

public:
    Game() : player1("Player 1"), player2("Player 2") {}

    void start() {
        std::cout << "Welcome to Battleship Blitz!\n";
        
        std::cout << "Player 1, place your ships:\n";
        player1.placeShips();

        std::cout << "Player 2, place your ships:\n";
        player2.placeShips();

        while (player1Attempts < MAX_ATTEMPTS && player2Attempts < MAX_ATTEMPTS) {
            std::cout << "Player 1's turn:\n";
            if (player1.makeMove(player2)) {
                if (player2.hasLost()) {
                    std::cout << "Player 1 wins!\n";
                    return;
                }
            }
            player1Attempts++;

            std::cout << "Player 2's turn:\n";
            if (player2.makeMove(player1)) {
                if (player1.hasLost()) {
                    std::cout << "Player 2 wins!\n";
                    return;
                }
            }
            player2Attempts++;
        }

        // Game ends after each player has made 5 moves
        std::cout << "Game Over!\n";
        std::cout << "Player 1 hits: " << player1.hits << "\n";
        std::cout << "Player 2 hits: " << player2.hits << "\n";

        if (player1.hits > player2.hits) {
            std::cout << "Player 1 wins by hitting more ships!\n";
        } else if (player2.hits > player1.hits) {
            std::cout << "Player 2 wins by hitting more ships!\n";
        } else {
            std::cout << "It's a draw!\n";
        }
    }
};

int main() {
    Game game;
    game.start();
    return 0;
}