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

#include "or_bus/or_frame.h"
#include "or_bus/or_message.h"

#define HASHMAP_NUMBER 4

typedef struct _frame_read {
    frame_reader_t send;
    frame_reader_t receive;
} frame_read_t;

typedef struct _hashmap {
    frame_read_t reader;
    unsigned char name;
} hashmap;

hashmap hash[HASHMAP_NUMBER];
unsigned short counter = 0;

/******************************************************************************/
/* Parsing functions                                                          */
/******************************************************************************/

void orb_frame_init() {
    unsigned short i;
    for(i = 0; i < HASHMAP_NUMBER; ++i) {
        hash[i].reader.send = NULL;
        hash[i].reader.receive = NULL;
        hash[i].name = 0;
    }
}

void set_frame_reader(unsigned char hashmap, frame_reader_t send, frame_reader_t receive) {
    frame_read_t frame;
    frame.send = send;
    frame.receive = receive;
    
    hash[counter].name = hashmap;
    hash[counter].reader = frame;
    counter++;
}

int get_key(unsigned char hashmap) {
    int i;
    for(i = 0; i < HASHMAP_NUMBER; ++i) {
        if(hashmap == hash[i].name) {
            return i;
        }
    }
    return -1;
}

bool parser(packet_t* receive_pkg, packet_information_t* list_to_send, size_t* len) {
    unsigned int i;
    for (i = 0; i < receive_pkg->length; i += receive_pkg->buffer[i]) {
        packet_information_t info;
        memcpy((unsigned char*) &info, &receive_pkg->buffer[i], receive_pkg->buffer[i]);
        int key = get_key(info.type);
        if(key != -1) {
            switch (info.option) {
                case PACKET_DATA:
                    hash[key].reader.receive(&list_to_send[0], len, &info);
                    break;
                case PACKET_REQUEST:
                    hash[key].reader.send(&list_to_send[0], len, &info);
                    break;
            }
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

inline packet_information_t createPacket(unsigned char command, unsigned char option, unsigned char type, message_abstract_u * packet, size_t len) {
    packet_information_t information;
    information.command = command;
    information.option = option;
    information.type = type;
    information.length = LNG_HEAD_INFORMATION_PACKET + len;
    if (packet != NULL) {
        memcpy(&information.message, packet, len);
    }
    return information;
}

inline packet_information_t createDataPacket(unsigned char command, unsigned char type, message_abstract_u * packet, size_t len) {
    return createPacket(command, PACKET_DATA, type, packet, len);
}