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

/******************************************************************************/
/* Definition commands                                                        */
/******************************************************************************/
//Name for HASHMAP with information about standard messages
#define HASHMAP_PERIPHERALS          'P'
/**
 * This union converts a command message in a command and a number port if required
 * - [#] maximum command 2^5 = 32
 * - [#] maximum port 2^3 = 8
 */
typedef union _peripheral_gpio_map {

    struct {
        unsigned char command : 5;    ///< Peripheral commnad
        unsigned char port : 3;       ///< Peripheral port
    } bitset;
    unsigned char message;
} peripheral_gpio_map_t;
//Number association for standard messages
#define PERIPHERALS_GPIO           0
#define PERIPHERALS_GPIO_SET       1
#define PERIPHERALS_GPIO_DIGITAL   2
#define PERIPHERALS_SERIAL         3
/******************************************************************************/
/* Definition messages                                                        */
/******************************************************************************/

typedef enum {
    PERIPHERAL_GPIO_INPUT  = 1,
    PERIPHERAL_GPIO_OUTPUT = 0,
    PERIPHERAL_GPIO_ANALOG = 2        
} peripheral_type_t;
/**
 * Number of the pin
 */
typedef uint8_t peripheral_gpio_number_t;
#define LNG_PERIPHERAL_GPIO sizeof(peripheral_gpio_number_t)
/**
 * Value of the pin
 */
typedef uint16_t peripherals_gpio_t;
#define LNG_PERIPHERALS_GPIO sizeof(peripherals_gpio_t)  
/**
 * Send the configuration off all digital ports
 * - [#]      Length of the port
 * - [0bXXXX] Binary value of the port
 */
typedef struct _peripherals_gpio_port {
    uint8_t len;
    peripherals_gpio_t port;
} peripherals_gpio_port_t;
#define LNG_PERIPHERALS_GPIO_PORT sizeof(peripherals_gpio_port_t)
/**
 * Configuration GPIO
 * - [0bXX...X]  Port GPIO to setup
 * - [0 - 2]     Configuration GPIO [0 Read, 1 Write, 2 Analog (if available)]
 */
typedef struct _peripherals_gpio_set {
    peripherals_gpio_port_t port;
    peripheral_type_t type;
} peripherals_gpio_set_t;
#define LNG_PERIPHERALS_GPIO_SET sizeof(peripherals_gpio_set_t)
/**
 * 
 */
typedef struct _peripherals_serial {
    uint8_t number;
    uint32_t baud;
    int16_t byte_conf;
} peripherals_serial_t;
#define LNG_PERIPHERALS_SERIAL sizeof(peripherals_serial_t)
/**
 * List of all system messages
 */
typedef union _peripherals_gpio_frame {
    peripheral_gpio_number_t number;
    peripherals_gpio_t pin;
    peripherals_gpio_set_t set;
    peripherals_gpio_port_t port;
    peripherals_serial_t serial;
} peripherals_gpio_frame_u;

#ifdef	__cplusplus
}
#endif

#endif	/* FRAME_PERIPHERALS_H */

