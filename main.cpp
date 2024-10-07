#include "robot_control.h"

int main() {
    // Example usage
    float wheel_base_width = 15.0;    // Distance between wheels in cm
    float wheel_diameter = 7.0;       // Wheel diameter in cm
    int encoder_counts_per_rev = 360; // Encoder counts per wheel revolution

    // Turn right (90 degrees)
    turnRobot(1, wheel_base_width, wheel_diameter, encoder_counts_per_rev);

    // Move forward 50 cm at max speed of 100
    moveForward(50.0, wheel_diameter, encoder_counts_per_rev, 100);

    // Turn left (90 degrees)
    turnRobot(-1, wheel_base_width, wheel_diameter, encoder_counts_per_rev);
    
    return 0;
}
