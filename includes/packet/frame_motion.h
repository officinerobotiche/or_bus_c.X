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

#ifndef FRAMEMOTION_H
#define	FRAMEMOTION_H

#include <stdint.h>

//+++++++++ NAVIGATION CONTROL ++++++++++++//

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
typedef struct _coordinate {
    float x;
    float y;
    float theta;
    float space;
} motion_coordinate_t;
#define LNG_MOTION_COORDINATE sizeof(motion_coordinate_t)

/**
 * Parameters definition for unicycle robot:
 * - radius (left and right)
 * - wheelbase
 * - minimal space for odometry
 */
typedef struct _parameter_unicycle {
    float radius_r;
    float radius_l;
    float wheelbase;
    float sp_min;
} motion_parameter_unicycle_t;
#define LNG_MOTION_PARAMETER_UNICYCLE sizeof(motion_parameter_unicycle_t)

/**
 * Message for read and write velocity in a unicycle robot:
 * - v = linear velocity
 * - w = angular velocity
 */
typedef struct _velocity {
    float v;
    float w;
} motion_velocity_t;
#define LNG_MOTION_VELOCITY sizeof(motion_velocity_t)

/**
 * Message for read and write state high level control
 */
typedef int8_t motion_state_t;
#define LNG_MOTION_STATE sizeof(motion_state_t)

/**
 * List of all motion messages
 */
typedef union _motion_frame {
    motion_coordinate_t coordinate;
    motion_parameter_unicycle_t parameter_unicycle;
    motion_velocity_t velocity;
    motion_state_t state;
} motion_frame_u;

//Numbers associated for motion messages
#define MOTION_COORDINATE 0
#define MOTION_VEL 1
#define MOTION_PARAMETER_UNICYCLE 2
#define MOTION_STATE 3
#define MOTION_VEL_REF 4

//Name for HASHMAP with information about motion messages
#define HASHMAP_MOTION 'M'
#define HASHMAP_MOTION_NUMBER 10

// Definition on communication/parsing_packet.c
//static unsigned int hashmap_motion[HASHMAP_MOTION_NUMBER];

/**
 * Table with conversion number message in a length for data messages
 */
#define HASHMAP_MOTION_INITIALIZE   hashmap_motion[MOTION_COORDINATE] = LNG_MOTION_COORDINATE;                   \
                                    hashmap_motion[MOTION_PARAMETER_UNICYCLE] = LNG_MOTION_PARAMETER_UNICYCLE;   \
                                    hashmap_motion[MOTION_VEL_REF] = LNG_MOTION_VELOCITY;                        \
                                    hashmap_motion[MOTION_STATE] = LNG_MOTION_STATE;                             \
                                    hashmap_motion[MOTION_VEL] = LNG_MOTION_VELOCITY;
                                    
#endif	/* FRAMEMOTION_H */

