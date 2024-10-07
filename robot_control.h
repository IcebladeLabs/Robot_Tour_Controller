#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H


// resetEncoders(), getLeftEncoder(), getRightEncoder(), setMotorSpeeds(), and stopMotors() would need to be adapted to hardware specific methods, as well as MAX_WHEEL_SPEED.


// Include necessary libraries
#include <cmath>

// PID control constants (can be adjusted based on tuning)
inline float Kp = 1.5;
inline float Ki = 0.0;
inline float Kd = 0.5;

// Variables for PID control (track previous state across function calls)
inline float prev_error = 0.0;
inline float integral = 0.0;

// Function Definitions

// PID control function
inline float PID(float target, float current) {
    // Calculate error
    float error = target - current;

    // Proportional term
    float P = Kp * error;

    // Integral term
    integral += error;
    float I = Ki * integral;

    // Derivative term
    float D = Kd * (error - prev_error);
    prev_error = error;

    // Return the PID control value
    return P + I + D;
}

// Function to reset PID (optional, to reset integral/prev_error between operations)
inline void resetPID() {
    prev_error = 0.0;
    integral = 0.0;
}

// Turn robot 90 degrees
inline void turnRobot(int direction, float wheel_base_width, float wheel_diameter, int encoder_counts_per_rev) {
    // Calculate the distance for a 90-degree turn
    float turn_distance = (M_PI * wheel_base_width) / 4; // Arc length for 90 degrees
    int encoder_target = (turn_distance / (M_PI * wheel_diameter)) * encoder_counts_per_rev;

    // Reset encoders
    resetEncoders();
    resetPID();

    while (true) {
        int left_encoder = getLeftEncoder();
        int right_encoder = getRightEncoder();

        if (direction == -1) {
            // Left turn
            int left_motor_speed = -PID(encoder_target, abs(left_encoder));
            int right_motor_speed = PID(encoder_target, abs(right_encoder));
            setMotorSpeeds(left_motor_speed, right_motor_speed);
        } else if (direction == 1) {
            // Right turn
            int left_motor_speed = PID(encoder_target, abs(left_encoder));
            int right_motor_speed = -PID(encoder_target, abs(right_encoder));
            setMotorSpeeds(left_motor_speed, right_motor_speed);
        }

        // Check if both encoders have reached the target
        if (abs(left_encoder) >= encoder_target && abs(right_encoder) >= encoder_target) {
            stopMotors();
            break;
        }
    }
}

// Move forward a specific distance
inline void moveForward(float distance_cm, float wheel_diameter, int encoder_counts_per_rev, float time_seconds) {
    // Calculate the target speed based on distance and time
    float speed_cm_per_sec = distance_cm / time_seconds; // Calculate speed in cm/s
    int target_speed = static_cast<int>(speed_cm_per_sec * 255.0 / MAX_WHEEL_SPEED); // Scale to PWM value

    // Calculate encoder target based on wheel circumference
    int encoder_target = static_cast<int>((distance_cm / (M_PI * wheel_diameter)) * encoder_counts_per_rev);

    // Reset encoders and PID state
    resetEncoders();
    resetPID();

    while (true) {
        int left_encoder = getLeftEncoder();
        int right_encoder = getRightEncoder();

        // PID control for each motor
        int left_motor_speed = PID(encoder_target, left_encoder);
        int right_motor_speed = PID(encoder_target, right_encoder);

        // Limit motor speed to the target speed
        left_motor_speed = std::min(left_motor_speed, target_speed);
        right_motor_speed = std::min(right_motor_speed, target_speed);

        // Set motor speeds
        setMotorSpeeds(left_motor_speed, right_motor_speed);

        // Stop when both encoders reach the target
        if (left_encoder >= encoder_target && right_encoder >= encoder_target) {
            stopMotors();
            break;
        }
    }
}

// Helper functions (need to be implemented based on your robot hardware)
// These should be hardware-specific to interface with the robot's motors and encoders
inline void resetEncoders() {
    // Reset encoder values to zero
    // Implement this according to your hardware
}

inline int getLeftEncoder() {
    // Return current left encoder count
    // Implement this according to your hardware
    return 0; // Placeholder
}

inline int getRightEncoder() {
    // Return current right encoder count
    // Implement this according to your hardware
    return 0; // Placeholder
}

inline void setMotorSpeeds(int left_motor_speed, int right_motor_speed) {
    // Set motor speeds (left and right)
    // Implement this according to your hardware
}

inline void stopMotors() {
    // Stop both motors
    // Implement this according to your hardware
}

#endif // ROBOT_CONTROL_H
