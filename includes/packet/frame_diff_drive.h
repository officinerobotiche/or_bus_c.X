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

#ifndef FRAMEDIFFDRIVE_H
#define	FRAMEDIFFDRIVE_H

#include <stdint.h>

//Name for HASHMAP with information about motion messages
#define HASHMAP_DIFF_DRIVE 'M'

/**
 * Define to select high state of control
 */
#define STATE_CONTROL_HIGH_DISABLE 0
#define STATE_CONTROL_HIGH_VELOCITY 1

/**
 * Definition for coordinate robot:
 * - position [x, y, theta]
 * - space
 */
typedef struct __attribute__ ((__packed__)) _coordinate {
    float x;
    float y;
    float theta;
    float space;
} diff_drive_coordinate_t;
#define LNG_DIFF_DRIVE_COORDINATE sizeof(diff_drive_coordinate_t)

/**
 * Parameters definition for unicycle robot:
 * - radius (left and right)
 * - wheelbase
 * - minimal space for odometry
 */
typedef struct __attribute__ ((__packed__)) _parameter_unicycle {
    float radius_r;
    float radius_l;
    float wheelbase;
    float sp_min;
} diff_drive_parameter_unicycle_t;
#define LNG_DIFF_DRIVE_PARAMETER_UNICYCLE sizeof(diff_drive_parameter_unicycle_t)

/**
 * Message for read and write velocity in a unicycle robot:
 * - v = linear velocity
 * - w = angular velocity
 */
typedef struct __attribute__ ((__packed__)) _velocity {
    float v;
    float w;
} diff_drive_velocity_t;
#define LNG_DIFF_DRIVE_VELOCITY sizeof(diff_drive_velocity_t)

/**
 * Message for read and write state high level control
 */
typedef int8_t diff_drive_state_t;
#define LNG_DIFF_DRIVE_STATE sizeof(diff_drive_state_t)

/**
 * List of all motion messages
 */
typedef union _motion_frame {
    diff_drive_coordinate_t coordinate;
    diff_drive_parameter_unicycle_t parameter_unicycle;
    diff_drive_velocity_t velocity;
    diff_drive_state_t state;
} diff_drive_frame_u;

//Numbers associated for motion messages
#define DIFF_DRIVE_COORDINATE 0
#define DIFF_DRIVE_VEL 1
#define DIFF_DRIVE_PARAMETER_UNICYCLE 2
#define DIFF_DRIVE_STATE 3
#define DIFF_DRIVE_VEL_REF 4
                                    
#endif	/* FRAMEDIFFDRIVE_H */

