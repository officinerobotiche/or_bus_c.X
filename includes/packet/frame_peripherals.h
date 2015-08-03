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

#ifndef FRAME_PERIPHERALS_H
#define	FRAME_PERIPHERALS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

//Name for HASHMAP with information about standard messages
#define HASHMAP_PERIPHERALS          'P'

/**
 * Configuration GPIO
 * - [#]     Number GPIO to set
 * - [0 - 2] Configuration GPIO [0 Read, 1 Write, 2 Analog (if available)]
 */
typedef struct _peripherals_gpio_set {
    uint8_t number;
    int8_t type;
} peripherals_gpio_set_t;
#define LNG_PERIPHERALS_GPIO_SET sizeof(peripherals_gpio_set_t)   

typedef struct _peripherals_gpio_analog {
    
} peripherals_gpio_analog_t;
#define LNG_PERIPHERALS_GPIO_ANALOG sizeof(peripherals_gpio_analog_t)

typedef int16_t peripherals_gpio_port_t;
#define LNG_PERIPHERALS_GPIO_PORT sizeof(peripherals_gpio_port_t) 

typedef struct _peripherals_serial {
    uint8_t number;
    int16_t baud;
    int16_t byte_conf;
} peripherals_serial_t;
#define LNG_PERIPHERALS_SERIAL sizeof(peripherals_serial_t)

/**
 * List of all system messages
 */
typedef union _peripherals_gpio_frame {
    peripherals_gpio_set_t set;
    peripherals_gpio_port_t port;
    peripherals_serial_t serial;
} peripherals_gpio_frame_u;

//Number association for standard messages
#define PERIPHERALS_GPIO_SET       0
#define PERIPHERALS_GPIO_ALL       1
#define PERIPHERALS_GPIO           2
#define PERIPHERALS_SERIAL         3
#define PERIPHERALS_TIMER          4

    
#ifdef	__cplusplus
}
#endif

#endif	/* FRAME_PERIPHERALS_H */

