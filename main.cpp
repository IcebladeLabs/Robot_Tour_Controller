#include "robot_control.h"
#include <vector>

int main() {

    float wheel_base_width = 15.0;    // Distance between wheels in cm
    float wheel_diameter = 7.0;       // Wheel diameter in cm
    int encoder_counts_per_rev = 360; // Encoder counts per wheel revolution
    double turntime = 3;
    double targettime = 65;
    std::vector<int> instructions = {0, 1, 0, 0, -1, 0, 1, -1, -1};

    // Iterate through the instructions and count the instructions
    
    int turns = 0;
    int forwards = 0;
    for (size_t i = 0; i < instructions.size(); ++i) {
        int value = instructions[i];
        if (value == -1) {
            turns++;
        } else if (value == 0) {
            forwards++;
        } else {
            turns++;
        }
    }

    // Calculate time for each operation, then iterate through again and run the ops
    targettime -= turntime * turns;
    double movetime = targettime / forwards;

    for (size_t i = 0; i < instructions.size(); ++i) {
        int value = instructions[i];
        if (value == -1) {
            turnRobot(-1, wheel_base_width, wheel_diameter, encoder_counts_per_rev);
        } else if (value == 0) {
            moveForward(25.0, wheel_diameter, encoder_counts_per_rev, movetime);
        } else {
            turnRobot(1, wheel_base_width, wheel_diameter, encoder_counts_per_rev);
        }
    }
    
    return 0;
}

