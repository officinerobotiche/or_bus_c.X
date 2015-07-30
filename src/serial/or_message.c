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

#include "serial/or_message.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/*! Pointer to function, initialized for pkg_header */
int (*pkg_parse) (unsigned char inchar) = &pkg_header;

/*! Receive packet */
packet_t* packet_receive;
unsigned int index_data = 0;
system_error_serial_t serial_error;

/******************************************************************************/
/* Communication Functions                                                    */
/******************************************************************************/

void orb_message_init(packet_t* packet_rx) {
    packet_receive = packet_rx;
    memset(serial_error.number, 0, MAX_BUFF_ERROR_SERIAL);
}

int decode_pkgs(unsigned char rxchar) {
    return (*pkg_parse)(rxchar);
}

int pkg_header(unsigned char rxchar) {
    if (rxchar == PACKET_HEADER) {
        pkg_parse = &pkg_length;
        return false;
    } else {
        return pkg_error(ERROR_HEADER);
    }
}

int pkg_length(unsigned char rxchar) {
    if (rxchar > MAX_BUFF_RX) {
        return pkg_error(ERROR_LENGTH);
    } else {
        pkg_parse = &pkg_data;
        packet_receive->length = rxchar;
        return false;
    }
}

int pkg_data(unsigned char rxchar) {
    int cks_clc;
    if ((index_data + 1) == (packet_receive->length + 1)) {
        pkg_parse = &pkg_header; //Restart parse serial packet
        if ((cks_clc = pkg_checksum(packet_receive->buffer, 0, index_data)) == rxchar) { //checksum data
            index_data = 0; //flush index array data buffer
            return true;
        } else {
            bool t = pkg_error(ERROR_CKS);
            return t;
        }
    } else {
        packet_receive->buffer[index_data] = rxchar;
        index_data++;
        return false;
    }
}

int pkg_error(int error) {
    index_data = 0;
    pkg_parse = &pkg_header; //Restart parse serial packet
    serial_error.number[(-error - 1)] += 1;
    return error;
}

unsigned char pkg_checksum(volatile unsigned char* Buffer, int FirstIndx, int LastIndx) {
    unsigned char ChkSum = 0;
    int ChkCnt;
    for (ChkCnt = FirstIndx; ChkCnt < LastIndx; ChkCnt++) {
        ChkSum += Buffer[ChkCnt];
    }
    return ChkSum;
}

void build_pkg(unsigned char * BufferTx, packet_t packet) {

    int i;
    BufferTx[0] = PACKET_HEADER;
    BufferTx[1] = packet.length;

    //Copy all element to DMA buffer
    for (i = 0; i < packet.length; i++) {
        BufferTx[i + LNG_PACKET_HEADER] = packet.buffer[i];
    }

    // Create a checksum
    BufferTx[packet.length + LNG_PACKET_HEADER] = pkg_checksum(BufferTx, LNG_PACKET_HEADER, packet.length + LNG_PACKET_HEADER);
}