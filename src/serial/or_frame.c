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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include <string.h>

#include "serial/or_frame.h"
#include "serial/or_message.h"

//Table to convertion name (number) of message in a length
//See packet/packet.h and packet/unav.h
static unsigned int hashmap_system[HASHMAP_SYSTEM_NUMBER];
static unsigned int hashmap_motor[HASHMAP_MOTOR_NUMBER];
static unsigned int hashmap_motion[HASHMAP_MOTION_NUMBER];

typedef struct _frame_read {
    frame_reader frame;
    unsigned char hashmap;
} frame_read_t;

frame_read_t save_frame;
frame_read_t send_frame;
    
/** GLOBAL VARIBLES */
// From serial/serial.c
extern system_error_serial_t serial_error;
extern packet_t receive_pkg;
extern char receive_header;

/******************************************************************************/
/* Parsing functions                                                          */
/******************************************************************************/

void init_hashmap() {
    save_frame.frame = NULL;
    send_frame.frame = NULL;
    HASHMAP_SYSTEM_INITIALIZE
    HASHMAP_MOTOR_INITIALIZE
    HASHMAP_MOTION_INITIALIZE
}

void set_frame_data(unsigned char hashmap, frame_reader save_f) {
    save_frame.frame = save_f;
    save_frame.hashmap = hashmap;
}

void set_frame_request(unsigned char hashmap, frame_reader save_f) {
    send_frame.frame = save_f;
    send_frame.hashmap = hashmap;
}

bool parser(packet_information_t* list_to_send, unsigned short* len) {
    unsigned int i;
    packet_information_t list_data[BUFFER_LIST_PARSING];
    unsigned short counter = 0;
    //Save single packet
    for (i = 0; i < receive_pkg.length; i += receive_pkg.buffer[i]) {
        memcpy((unsigned char*) &list_data[counter++], &receive_pkg.buffer[i], receive_pkg.buffer[i]);
    }
    //Compute packet
    for (i = 0; i < counter; ++i) {
        packet_information_t* info = &list_data[i];
        switch (info->option) {
            case PACKET_DATA:
                if(save_frame.hashmap == info->type) {
                    save_frame.frame(&list_to_send[0], i, info);
                }
                break;
            case PACKET_REQUEST:
                if(save_frame.hashmap == info->type) {
                    send_frame.frame(&list_to_send[0], i, info);
                }
                break;
        }
    }
    return true;
}

packet_t encoder(packet_information_t *list_send, size_t len) {
    int i;
    packet_t packet_send;
    packet_send.length = 0;
    for (i = 0; i < len; ++i) {
        packet_buffer_u buffer_packet;
        buffer_packet.packet_information = list_send[i];

        memcpy(&packet_send.buffer[packet_send.length], &buffer_packet.buffer, buffer_packet.packet_information.length);

        packet_send.length += buffer_packet.packet_information.length;
    }
    return packet_send;
}

packet_t encoderSingle(packet_information_t send) {
    packet_t packet_send;
    packet_send.length = send.length + 1;
    packet_buffer_u buffer_packet;
    buffer_packet.packet_information = send;
    memcpy(&packet_send.buffer, &buffer_packet.buffer, buffer_packet.packet_information.length + 1);
    return packet_send;
}

packet_information_t createPacket(unsigned char command, unsigned char option, unsigned char type, message_abstract_u * packet) {
    packet_information_t information;
    information.command = command;
    information.option = option;
    information.type = type;
    motor_command_map_t command_motor;
    if (option == PACKET_DATA) {
        switch (type) {
            case HASHMAP_SYSTEM:
                information.length = LNG_HEAD_INFORMATION_PACKET + hashmap_system[command];
                break;
            case HASHMAP_MOTION:
                information.length = LNG_HEAD_INFORMATION_PACKET + hashmap_motion[command];
                break;
            case HASHMAP_MOTOR:
                command_motor.command_message = command;
                information.length = LNG_HEAD_INFORMATION_PACKET + hashmap_motor[command_motor.bitset.command];
                break;
            default:
                //TODO throw
                break;
        }
    } else {
        information.length = LNG_HEAD_INFORMATION_PACKET;
    }
    if (packet != NULL) {
        memcpy(&information.message, packet, sizeof (message_abstract_u));
    }
    return information;
}

packet_information_t createDataPacket(unsigned char command, unsigned char type, message_abstract_u * packet) {
    return createPacket(command, PACKET_DATA, type, packet);
}
