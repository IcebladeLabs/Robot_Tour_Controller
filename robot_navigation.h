#ifndef ROBOT_NAVIGATION_H
#define ROBOT_NAVIGATION_H

#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

// Define the size of the grid
const int ROWS = 11;
const int COLS = 9;

// Structure to represent a position in the grid
struct Position {
    int x, y;
};

// Function to find the Start (2), Intermediary Gates (3), and End Point (4)
std::pair<std::vector<Position>, std::pair<Position, Position>> findStartGatesAndEnd(int grid[ROWS][COLS]) {
    std::vector<Position> gates;
    Position start = {-1, -1};
    Position end = {-1, -1};

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 2) {
                start = {i, j};  // Start point
            } else if (grid[i][j] == 3) {
                gates.push_back({i, j});  // Intermediary gates
            } else if (grid[i][j] == 4) {
                end = {i, j};  // End point
            }
        }
    }

    return {gates, {start, end}};
}

// Function to calculate the path from one point to another using BFS
std::vector<int> bfsPath(int grid[ROWS][COLS], Position start, Position end) {
    std::vector<std::vector<bool>> visited(ROWS, std::vector<bool>(COLS, false));
    std::vector<std::vector<Position>> parent(ROWS, std::vector<Position>(COLS, {-1, -1}));
    std::queue<Position> q;

    // Direction vectors: right, down, left, up
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    // Initialize the BFS from the starting point
    q.push(start);
    visited[start.x][start.y] = true;

    // Perform BFS
    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        // If we reached the end position, break
        if (current.x == end.x && current.y == end.y) break;

        // Explore neighbors (right, down, left, up)
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && grid[newX][newY] != 0 && !visited[newX][newY]) {
                visited[newX][newY] = true;
                parent[newX][newY] = current;  // Record parent to reconstruct path later
                q.push({newX, newY});
            }
        }
    }

    // Backtrack to find the path from start to end
    std::vector<Position> path;
    Position current = end;
    while (!(current.x == start.x && current.y == start.y)) {
        path.push_back(current);
        current = parent[current.x][current.y];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    // Translate the path into movement instructions
    std::vector<int> instructions;
    int direction = 0;  // Assume the robot starts facing right (0 = right, 1 = down, 2 = left, 3 = up)

    for (size_t i = 1; i < path.size(); i++) {
        int deltaX = path[i].x - path[i - 1].x;
        int deltaY = path[i].y - path[i - 1].y;

        // Determine the new direction based on the movement (right, down, left, up)
        int newDirection = -1;
        if (deltaX == 0 && deltaY == 1) newDirection = 0;  // Right
        else if (deltaX == 1 && deltaY == 0) newDirection = 1;  // Down
        else if (deltaX == 0 && deltaY == -1) newDirection = 2;  // Left
        else if (deltaX == -1 && deltaY == 0) newDirection = 3;  // Up

        // Determine the rotation needed to match the new direction
        int rotation = (newDirection - direction + 4) % 4;
        if (rotation == 1) instructions.push_back(1);  // Right turn
        else if (rotation == 3) instructions.push_back(-1);  // Left turn

        // Move forward
        instructions.push_back(0);

        // Update the direction to the new one
        direction = newDirection;
    }

    return instructions;
}

// Helper function to calculate the distance between two points (Manhattan distance)
int calculateDistance(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Function to determine the order to visit all gates ('3's) efficiently using nearest-neighbor heuristic
std::vector<Position> findEfficientPathForGates(const std::vector<Position>& gates, Position start) {
    std::vector<Position> orderedPath;
    std::vector<bool> visited(gates.size(), false);

    Position current = start;  // Start from the start point
    orderedPath.push_back(current);

    // Iterate through all gate positions to find the most efficient path
    for (size_t i = 0; i < gates.size(); i++) {
        int nearestIdx = -1;
        int nearestDist = INT_MAX;

        for (size_t j = 0; j < gates.size(); j++) {
            if (!visited[j]) {
                int dist = calculateDistance(current, gates[j]);
                if (dist < nearestDist) {
                    nearestDist = dist;
                    nearestIdx = j;
                }
            }
        }

        // Move to the nearest unvisited gate
        current = gates[nearestIdx];
        orderedPath.push_back(current);
        visited[nearestIdx] = true;
    }

    return orderedPath;
}

// Function to move from start (2) through all gates (3), and then to the endpoint (4)
std::vector<int> findPath(int grid[ROWS][COLS]) {
    std::vector<int> totalInstructions; // To store the overall instructions

    // Get the start point (2), intermediary gates (3), and endpoint (4)
    auto [gates, startEnd] = findStartGatesAndEnd(grid);
    Position start = startEnd.first;
    Position end = startEnd.second;

    // Get the efficient path through all gates
    std::vector<Position> orderedGates = findEfficientPathForGates(gates, start);

    // Move through all gates
    for (size_t i = 0; i < orderedGates.size() - 1; i++) {
        std::vector<int> pathInstructions = bfsPath(grid, orderedGates[i], orderedGates[i + 1]);
        totalInstructions.insert(totalInstructions.end(), pathInstructions.begin(), pathInstructions.end());
    }

    // After visiting all gates, move to the '4' endpoint
    std::vector<int> finalPathInstructions = bfsPath(grid, orderedGates.back(), end);
    totalInstructions.insert(totalInstructions.end(), finalPathInstructions.begin(), finalPathInstructions.end());

    return totalInstructions; // Return the complete list of instructions
}

#endif // ROBOT_NAVIGATION_H
