/*
 * Copyright (C) 2015 Officine Robotiche
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

#ifndef FRAME_GPIO_H
#define	FRAME_GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

//Name for HASHMAP with information about standard messages
#define HASHMAP_GPIO          'P'

/**
 * Configuration GPIO
 * - [#]     Number GPIO to set
 * - [0 - 2] Configuration GPIO [1 Read, 2 Write, 3 Analog (if available)]
 */
typedef struct _gpio_set {
    uint8_t number;
    int8_t type;
} gpio_set_t;
#define LNG_GPIO_SET sizeof(gpio_set_t)   

typedef struct _gpio_analog {
    
} gpio_analog_t;

typedef int16_t gpio_port_t;
#define LNG_GPIO_PORT sizeof(gpio_port_t) 

/**
 * List of all system messages
 */
typedef union _gpio_frame {
    gpio_set_t set;
    gpio_port_t port;
} gpio_frame_u;
    
//Number association for standard messages
#define GPIO_SET       0
#define GPIO_ALL       1
#define GPIO           2

    
#ifdef	__cplusplus
}
#endif

#endif	/* FRAME_GPIO_H */

