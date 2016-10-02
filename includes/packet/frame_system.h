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

#ifndef FRAMESYSTEM_H
#define	FRAMESYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//Name for HASHMAP with information about standard messages
#define HASHMAP_SYSTEM          'S'
    
// Type of serial errors
#define MAX_BUFF_ERROR_SERIAL 13
// Numbers of process names 
#define MAX_BUFF_TASK_NAME 20
// Dimension services buffer
#define MAX_BUFF_SERVICE 20
    
/**
 * Services messages for control board:
 * String with information about board and other
 */
typedef unsigned char system_service_t[MAX_BUFF_SERVICE];
#define LNG_SYSTEM_SERVICE sizeof(system_service_t)

/**
 * Service messages about number of error on serial communication
 * see on communication/serial.h type of errors
 */
typedef int16_t system_error_serial_t[MAX_BUFF_ERROR_SERIAL];
#define LNG_SYSTEM_ERROR_SERIAL sizeof(system_error_serial_t)

/**
 * - [#]   Time in idle
 * - [nS]  Time to use the ADC
 * - [nS]  time to manage all system led
 * - [nS]  Time to parse a packet
 * - [nS]  Time to read and write from I2C
 */
typedef struct _system_events {
    uint32_t idle;
    uint32_t adc;
    uint32_t led;
    uint32_t parser;
    uint32_t i2c;
} system_time_t;
#define LNG_SYSTEM_TIME sizeof(system_time_t)

// TO BE CHECK =========================================
    
///**
// * Define messages about parameter system:
// * * Clock of System timer
// * * Clock in milliseconds
// */
//typedef struct _system_parameter {
//    uint32_t FREQ_CPU;
//    uint32_t FREQ_SYSTEM;
//} system_parameter_t;
//#define LNG_SYSTEM_PARAMETER sizeof(system_parameter_t)
//
///**
// * Information about processes running on board. We have standards process:
// * * time in idle
// * * time for parsing packet
// * * list for others processes
// */
//typedef struct _system_task_name {
//    uint8_t number;
//    char data[MAX_BUFF_TASK_NAME];
//} system_task_name_t;
//#define LNG_SYSTEM_TASK_NAME sizeof(system_task_name_t)

/**
 * List of all system messages
 */
typedef union _system_frame {
    system_service_t service;
    system_error_serial_t error_serial;
    system_time_t time;
} system_frame_u;

//Number association for standard messages
#define SYSTEM_RESET           '*'
#define SYSTEM_CODE_DATE       'd'
#define SYSTEM_CODE_VERSION    'v'
#define SYSTEM_CODE_AUTHOR     'a'
#define SYSTEM_CODE_BOARD_TYPE 't'
#define SYSTEM_CODE_BOARD_NAME 'n'
#define SYSTEM_SERIAL_ERROR     0
#define SYSTEM_TIME             1

#ifdef	__cplusplus
}
#endif

#endif	/* FRAMESYSTEM_H */

