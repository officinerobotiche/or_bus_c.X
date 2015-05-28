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

#ifndef PACKET_H
#define	PACKET_H

#include <stdint.h>

/** Serial packets
 * On this header file, we have all definition about communication and defined
 * the standard packets, to send information :
 * * Parameter system
 * * error packages receives from serial ports
 * * Services messages
 *      * reset board
 *      * date code
 *      * name board
 *      * version code
 *      * author code
 * * Messages with all names processes
 * * Information about processes
 *
 * Other messages for this board are in packet/unav.h
 */

/*******/

#include "packet/frame_motion.h"
#include "packet/frame_motor.h"
#include "packet/frame_system.h"
#include "packet/frame_navigation.h"

/** Buffer dimensions */
// Dimension for UART transmit buffer
#define MAX_BUFF_TX 200
// Dimension for UART receive buffer
#define MAX_BUFF_RX 200

/** Type of option messages */
// Request data
#define PACKET_REQUEST  'R'
// Messages with data
#define PACKET_DATA     'D'
// ACK
#define PACKET_ACK      'K'
// NACK
#define PACKET_NACK     'N'
// Length of information packet (without data)
#define LNG_HEAD_INFORMATION_PACKET 4

/**
 * This is a definition to convert packets in a big data packet to send in 
 * a serial communication. 
 * For all packet we have this transformation:
 * 1. UNION abstract_packet_u
 * 2. STRUCT information_packet_t
 * 3. UNION buffer_packet_u
 *
 * Finally a function convert all information_packet_t in a
 * long data packet with name:
 * 4. STRUCT packet_t
 */

/**
 * Union for conversion all type of packets in a standard packets
 */
typedef union _message_abstract {
    system_frame_u system;
    motor_frame_u motor;
    motion_frame_u motion;
    navigation_frame_u sensor;
} message_abstract_u;

/**
 * Structure with information about a packet:
 * * length for packet
 * * information about packet (in top on this file):
 *      * (R) Request data
 *      * (D) Packet with data
 *      * (K) ACK
 *      * (N) NACK
 * * type packet:
 *      * (D) Default messages (in top on this file)
 *      * other type messages (in UNAV file)
 * * command message
 */
typedef struct _packet_information {
    unsigned char length;
    unsigned char option;
    unsigned char type;
    unsigned char command;
    message_abstract_u message;
} packet_information_t;

/**
 * Union to quickly transform information_packet_t in a buffer to add in
 * packet_t
 */
typedef union _packet_buffer {
    packet_information_t packet_information;
    unsigned char buffer[MAX_BUFF_RX];
} packet_buffer_u;

/**
 * Structure with information about packet to send with serial port:
 * * length of packet
 * * buffer with data
 * * time to send packet (NOT IN USE)
 */
typedef struct _packet {
    unsigned int length;
    unsigned char buffer[MAX_BUFF_RX];
    unsigned int time;
} packet_t;

#endif	/* PACKET_H */

