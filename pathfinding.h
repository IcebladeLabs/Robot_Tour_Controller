#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include <queue>
#include <iostream>

const int ROWS = 11;
const int COLS = 9;
const int DIRECTIONS[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

struct Node {
    int row, col, dist;
    std::vector<int> path;
    Node(int r, int c, int d, std::vector<int> p) : row(r), col(c), dist(d), path(p) {}
};

bool isValid(int row, int col, const int grid[ROWS][COLS], bool visited[ROWS][COLS]) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS && grid[row][col] != 0 && !visited[row][col];
}

// Breadth-First Search for the shortest path from start to target
std::vector<int> bfs(int grid[ROWS][COLS], int startRow, int startCol, int targetValue) {
    bool visited[ROWS][COLS] = {false};
    std::queue<Node> q;
    q.push(Node(startRow, startCol, 0, {}));
    visited[startRow][startCol] = true;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        if (grid[current.row][current.col] == targetValue) {
            return current.path;
        }

        for (int i = 0; i < 4; ++i) {
            int newRow = current.row + DIRECTIONS[i][0];
            int newCol = current.col + DIRECTIONS[i][1];
            if (isValid(newRow, newCol, grid, visited)) {
                visited[newRow][newCol] = true;
                std::vector<int> newPath = current.path;

                // Determine direction: Left (-1), Forward (0), Right (1)
                if (i == 0) newPath.push_back(0);  // Up -> Forward
                else if (i == 1) newPath.push_back(0);  // Down -> Forward
                else if (i == 2) newPath.push_back(-1); // Left -> Turn Left
                else if (i == 3) newPath.push_back(1);  // Right -> Turn Right

                q.push(Node(newRow, newCol, current.dist + 1, newPath));
            }
        }
    }
    return {}; // Return empty path if no path is found
}

// Main function to handle progression from 2 -> all 3s -> 4 -> 5
std::vector<int> findPath(int grid[ROWS][COLS]) {
    std::vector<int> completePath;

    // Step 1: Find starting point (2)
    int startRow = -1, startCol = -1;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c] == 2) {
                startRow = r;
                startCol = c;
                break;
            }
        }
        if (startRow != -1) break;
    }

    // Step 2: Progress through all 3s in order, then 4, then 5
    for (int targetValue = 3; targetValue <= 5; ++targetValue) {
        std::vector<int> pathToTarget = bfs(grid, startRow, startCol, targetValue);
        completePath.insert(completePath.end(), pathToTarget.begin(), pathToTarget.end());

        // Update start position to the last found target for the next iteration
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                if (grid[r][c] == targetValue) {
                    startRow = r;
                    startCol = c;
                    break;
                }
            }
            if (startRow != -1) break;
        }
    }

    return completePath;
}

#endif
