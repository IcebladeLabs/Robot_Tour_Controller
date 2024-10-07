#include "robot_control.h"
#include "pathfinding.h"

int main() {
    // Define the grid array (example: 11 rows, 9 columns)
    int grid[ROWS][COLS] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 3, 1, 1, 1, 1, 0, 1, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 5, 0 },
        { 0, 0, 0, 1, 1, 1, 0, 1, 0 },
        { 2, 1, 0, 1, 1, 1, 0, 3, 0 },
        { 0, 1, 0, 1, 1, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 1, 1, 4, 0, 1, 1, 3, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    // Get the instructions to move through all '2's and reach '3'
    std::vector<int> instructions = findPath(grid);

    // Print the instructions
    for (int instruction : instructions) {
        std::cout << instruction << " ";
    }
    std::cout << std::endl;

    return 0;
}

