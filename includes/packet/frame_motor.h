/*
 * Copyright (C) 2014 Officine Robotiche
 * Author: Raffaello Bonghi
 * email:  raffaello.bonghi@officinerobotiche.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU Lesser General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#ifndef FRAMEMOTOR_H
#define	FRAMEMOTOR_H

#include <stdint.h>

//Name for HASHMAP with information about motion messages
#define HASHMAP_MOTOR 'G'

/**
 * Define to select state of control for single motor
 */
#define STATE_CONTROL_EMERGENCY     -1  ///< Motors slow down to zero speed, then the bridge is turned off
#define STATE_CONTROL_DISABLE       0   ///< Motors disabled
#define STATE_CONTROL_POSITION      1   ///< Motors controlled in position
#define STATE_CONTROL_VELOCITY      2   ///< Motors controlled in velocity
#define STATE_CONTROL_CURRENT       3   ///< Motors controller in torque
#define STATE_CONTROL_DIRECT        4   ///< Motors controlled using direct PWM signals

/**
 * This union converts a command message in a motor index and type of command
 * - [#] maximum motor 2^3 = 8
 * - [#] maximum command 2^5 = 32
 */
typedef union _motor_command_map {

    struct {
        unsigned char motor : 3;    ///< Motor index
        unsigned char command : 5;  ///< Motor command
    } bitset;
    unsigned char command_message;
} motor_command_map_t;

/**
 * Message to control single motor
 * - [X] command or measure to control [physic dimension depends to type of command]
 */
#define MOTOR_CONTROL_MAX INT32_MAX
#define MOTOR_CONTROL_MIN INT32_MIN
typedef int32_t motor_control_t;
#define LNG_MOTOR_CONTROL sizeof(motor_control_t)

/**
 * Message to get status of a single motor
 * - [#] state of control
 */
typedef int8_t motor_state_t;
#define LNG_MOTOR_STATE sizeof(motor_state_t)

/**
 * Message for the status of the motor controller, information about:
 * - [#]       state motor - type of control
 * - [*]       Value of PWM applied
 * - [m Nm]    Value of drive torque
 * - [m A]     Current
 * - [m rad/s] velocity
 * - [rad]     position
 * - [rad]     delta position
 */
typedef struct __attribute__ ((__packed__)) _motor {
    motor_state_t state;
    motor_control_t pwm;
    motor_control_t effort;
    motor_control_t current;
    motor_control_t velocity;
    float position;
    float position_delta;
} motor_t;
#define LNG_MOTOR sizeof(motor_t)

/**
 * All diagnostic information about state of motor
 * - [mW]  Absorbed power motor
 * - [mV]  mean voltage applied in the bridge - PWM
 * - [m°C] Temperature motor
 * - [nS]  Time of execution control routine
 */
typedef struct __attribute__ ((__packed__)) _motor_diagnostic {
    int32_t watt;
    uint16_t volt;
    uint16_t temperature;
    uint32_t time_control;
} motor_diagnostic_t;
#define LNG_MOTOR_DIAGNOSTIC sizeof(motor_diagnostic_t)

/**
 * Encoder type definition:
 * - [ 0, 1]    Position encoder respect to gear [0 before, 1 after]
 * - [ 0, 1]    Z-index [0 false, 1 true]
 * - [ 0, 1]    Channel [0 One channel, 1 Two channels]
 */
#define MOTOR_ENC_BEFORE_GEAR 0
#define MOTOR_ENC_AFTER_GEAR 1
#define MOTOR_ENC_Z_INDEX_NO 0
#define MOTOR_ENC_Z_INDEX_YES 1
#define MOTOR_ENC_CHANNEL_ONE 0
#define MOTOR_ENC_CHANNEL_TWO 1
typedef struct _encoder_type {
        uint8_t position;
        uint8_t z_index;
        uint8_t channels;
        uint8_t          : 5;
} motor_encoder_type_t;
/**
 * Encoder parameters definition:
 * - [#]     Encoder CPR
 * Encoder type definition
 */
typedef struct __attribute__ ((__packed__)) _motor_parameter_encoder {
    uint16_t cpr;
    motor_encoder_type_t type;
} motor_parameter_encoder_t;
#define LNG_MOTOR_PARAMETER_ENCODER sizeof(motor_parameter_encoder_t)

/**
 * Parameters definition for motor:
 * - [ 0, 1] Default logic value to enable the H-bridge [0 low, 1 high]
 * - [XXX]   TODO
 * - [XXX]   TODO
 * - [V]     Voltage sensor gain 
 * - [V]     Current sense offset in volt
 * - [V/A]   Current sense gain in Volt Ampere
 */
#define MOTOR_ENABLE_LOW 0
#define MOTOR_ENABLE_HIGH 1
typedef struct __attribute__ ((__packed__)) _motor_parameter_bridge {
    uint8_t enable;
    uint16_t pwm_dead_zone;
    uint16_t pwm_frequency;
    float volt_offset;
    float volt_gain;
    float current_offset;
    float current_gain;
} motor_parameter_bridge_t;
#define LNG_MOTOR_PARAMETER_BRIDGE sizeof(motor_parameter_bridge_t)
/**
 * Collection of parameters to configure bridge and encoder
 * - Bridge configuration parameters
 * - Encoder parameters
 * - [-1, 1] Positive versus of the rotation of the motor [-1 clockwise, 1 counterclockwise]
 * - [#]     Gear ratio
 */
#define MOTOR_ROTATION_CLOCKWISE -1
#define MOTOR_ROTATION_COUNTERCLOCKWISE 1
typedef struct __attribute__ ((__packed__)) _motor_parameter {
    float ratio;
    int8_t rotation;
    motor_parameter_bridge_t bridge;
    motor_parameter_encoder_t encoder;
} motor_parameter_t;
#define LNG_MOTOR_PARAMETER sizeof(motor_parameter_t)

/**
 * Message for emergency configuration
 * - [s]  Time to put velocity motor to zero
 * - [s]  Time to disable bridge after the speed reaches velocity is zero
 * - [ms] Timeout to start emergency stop of the motors
 */
typedef struct __attribute__ ((__packed__)) _motor_emergency {
    float slope_time;
    float bridge_off;
    uint16_t timeout;
} motor_emergency_t;
#define LNG_MOTOR_EMERGENCY sizeof(motor_emergency_t)

/**
 * Message to define the gains for a PID controller
 * - [X] K_p  [Gain Proportional- physic dimension depends to type of control]
 * - [X] K_i  [Gain Integrative - physic dimension depends to type of control]
 * - [X] K_d  [Gain Derivative  - physic dimension depends to type of control]
 * - [X] K_aw [Gain Anti Wind-up- physic dimension depends to type of control]
 * - [Hz] Frequency of controller
 * - [true, false] Run the pid controller
 */
typedef struct __attribute__ ((__packed__)) _motor_pid {
    float kp;
    float ki;
    float kd;
    float kaw;
    uint32_t frequency;
    uint8_t enable;
} motor_pid_t;
#define LNG_MOTOR_PID sizeof(motor_pid_t)
/**
 * List of all motor messages
 */
typedef union _motor_frame {
    motor_t motor;
    motor_diagnostic_t diagnostic;
    motor_parameter_t parameter;
    motor_state_t state;
    motor_emergency_t emergency;
    motor_pid_t pid;
    motor_control_t reference;
} motor_frame_u;

//Numbers associated for motor messages to be used in the structure @ref motor_command_map_t as value for @ref command
#define MOTOR_MEASURE             0 ///< TODO Explain what this means
#define MOTOR_REFERENCE           1 ///< TODO Explain what this means
#define MOTOR_CONTROL             2 ///< TODO Explain what this means
#define MOTOR_DIAGNOSTIC          3 ///< TODO Explain what this means
#define MOTOR_PARAMETER           4 ///< TODO Explain what this means
#define MOTOR_CONSTRAINT          5 ///< TODO Explain what this means
#define MOTOR_EMERGENCY           6 ///< TODO Explain what this means
#define MOTOR_STATE               7 ///< TODO Explain what this means
#define MOTOR_POS_RESET           8 ///< TODO Explain what this means
#define MOTOR_POS_PID             9 ///< TODO Explain what this means
#define MOTOR_POS_REF            10 ///< TODO Explain what this means
#define MOTOR_VEL_PID            11 ///< TODO Explain what this means
#define MOTOR_VEL_REF            12 ///< TODO Explain what this means
#define MOTOR_CURRENT_PID        13 ///< TODO Explain what this means
#define MOTOR_CURRENT_REF        14 ///< TODO Explain what this means
#define MOTOR_TORQUE_REF         15 ///< TODO Explain what this means

#endif	/* FRAMEMOTOR_H */