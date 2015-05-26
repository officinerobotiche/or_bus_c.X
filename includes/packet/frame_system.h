/* 
 * File:   system.h
 * Author: Raffaello
 *
 * Created on May 25, 2015, 4:38 PM
 */

#ifndef FRAME_SYSTEM_H
#define	FRAME_SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// Type of serial errors
#define MAX_BUFF_ERROR_SERIAL 13
// Numbers of process names 
#define MAX_BUFF_TASK_NAME 20
// Dimension services buffer
#define MAX_BUFF_SERVICE 20

/**
 * Define messages about parameter system:
     * * Clock of System timer
     * * Clock in milliseconds
 */
typedef struct _system_parameter {
    int16_t step_timer;
    int16_t int_tm_mill;
} system_parameter_t;
#define LNG_SYSTEM_PARAMETER sizeof(system_parameter_t)

/**
 * Service messages about number of error on serial communication
 * see on communication/serial.h type of errors
 */
typedef struct _system_error_serial {
    int16_t number[MAX_BUFF_ERROR_SERIAL];
} system_error_serial_t;
#define LNG_SYSTEM_ERROR_SERIAL sizeof(system_error_serial_t)

/**
 * Services messages for control board:
 * * reset board
 * * date code
 * * name board
 * * version code
 * * author code
 */
typedef struct _system_service {
    char command; // TODO insert a list of commands or a reference to the list of commands
    unsigned char buffer[MAX_BUFF_SERVICE];
} system_service_t;
#define LNG_SYSTEM_SERVICE sizeof(system_service_t)

/**
 * Information about task running on firmware
 * * HASHMAP type of task
 * * number of task
 * * data required or read
 */
typedef struct _system_task {
    uint8_t hashmap;
    uint8_t number;
    uint8_t data;
} system_task_t;
#define LNG_SYSTEM_TASK sizeof(system_task_t)

/**
 * Information about processes running on board. We have standards process:
 * * time in idle
 * * time for parsing packet
 * * list for others processes
 */
typedef struct _system_task_name {
    uint8_t hashmap;
    uint8_t number;
    char data[MAX_BUFF_TASK_NAME];
} system_task_name_t;
#define LNG_SYSTEM_TASK_NAME sizeof(system_task_name_t)

//List of all system messages
#define ABSTRACT_MESSAGE_SYSTEM system_task_name_t system_task_name;          \
                                system_task_t system_task;                    \
                                system_service_t system_service;              \
                                system_error_serial_t system_error_serial;    \
                                system_parameter_t system_parameter;

//Number association for standard messages
#define SYSTEM_SERVICE          0
#define SYSTEM_TASK_NAME        1
#define SYSTEM_TASK_TIME        2
#define SYSTEM_TASK_PRIORITY    3
#define SYSTEM_TASK_FRQ         4
#define SYSTEM_TASK_NUM         5
#define SYSTEM_PARAMETER        6
#define SYSTEM_SERIAL_ERROR     7

//Names for type services
#define SERVICE_RESET           '*'
#define SERVICE_CODE_DATE       'd'
#define SERVICE_CODE_VERSION    'v'
#define SERVICE_CODE_AUTHOR     'a'
#define SERVICE_CODE_BOARD_TYPE 't'
#define SERVICE_CODE_BOARD_NAME 'n'

//Name for HASHMAP with information about standard messages
#define HASHMAP_SYSTEM          'S'
#define HASHMAP_SYSTEM_NUMBER   10

// Definition on communication/parsing_packet.c
//static unsigned int hashmap_system[HASHMAP_SYSTEM_NUMBER];

/**
 * Table with conversion number message in a length for data messages
 */
#define HASHMAP_SYSTEM_INITIALIZE hashmap_system[SYSTEM_SERVICE] = LNG_SYSTEM_SERVICE;           \
                                  hashmap_system[SYSTEM_TASK_NAME] = LNG_SYSTEM_TASK_NAME;       \
                                  hashmap_system[SYSTEM_TASK_TIME] = LNG_SYSTEM_TASK;            \
                                  hashmap_system[SYSTEM_TASK_PRIORITY] = LNG_SYSTEM_TASK;        \
                                  hashmap_system[SYSTEM_TASK_FRQ] = LNG_SYSTEM_TASK;             \
                                  hashmap_system[SYSTEM_TASK_NUM] = LNG_SYSTEM_TASK;             \
                                  hashmap_system[SYSTEM_PARAMETER] = LNG_SYSTEM_PARAMETER;       \
                                  hashmap_system[SYSTEM_SERIAL_ERROR] = LNG_SYSTEM_ERROR_SERIAL;

#ifdef	__cplusplus
}
#endif

#endif	/* FRAME_SYSTEM_H */

