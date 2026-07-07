# Battleship Blitz

A console implementation of the classic **Battleship** game in C++. Two players
place ships on a 10×10 grid and take turns firing at each other's board until the
attempt limit is reached.

## Gameplay

- **Board:** 10×10 grid, coordinates `0–9` on each axis.
- **Cells:** `-` water, `X` hit, `M` miss (ships are hidden on the opponent's board).
- **Attempts:** each player gets a fixed number of attacks (`MAX_ATTEMPTS = 5`).
- Players enter `x y` coordinates to fire; the game reports hits and misses and
  keeps the two boards separate.

## Design

The program is organized into small classes:

- `Coordinate` — an `(x, y)` grid position.
- `Ship` — a named ship of a given size with its occupied coordinates.
- `Board` — owns the grid and its ships, and handles display and attack logic.

## Build & Run

```bash
g++ Battleship.cpp -o battleship
./battleship
```

Requires any C++11-capable compiler (`g++` or `clang++`).
