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

#include "or_bus/frame.h"

/******************************************************************************/
/* Parsing functions                                                          */
/******************************************************************************/
/**
 * In a packet we have more frame. A typical data packet have this structure:
 * -------------------------- ---------------------------- --------------------
 * | LNG | TYPE | HASH | CMD | --- DATA --- | ... | LNG | TYPE | HASH | CMD | .
 * -------------------------- ---------------------------- --------------------
 *   [0]    [1]    [2]   [3]      [4]...[n]   ...   [i]   [i+1]  [i+2]  [i+3]
 * We have this struct of message
 * [i]   Length
 * [i+1] Type
 * [i+2] Hash
 * [i+3] Command
 * [i+4]...[i+n] Message
 * @param obj The O
 * @param buffer
 * @param size
 */
inline void OR_BUS_FRAME_decoder(void* obj, unsigned char *buffer, size_t size) {
    OR_BUS_FRAME_t *frame = (OR_BUS_FRAME_t*) obj;
    unsigned int i, IdxHash;
    // In the first place is located the size of the frame message
    for (i = 0; i < size; i += buffer[i]) {
        // Find the associated callback
        for (IdxHash = 0; IdxHash < OR_BUS_FRAME_LNG_HASH_DECODER; ++IdxHash) {
            // Run the associated callback
            if (frame->hash[IdxHash].pointer != NULL) {
                // Check the type
                if (frame->hash[IdxHash].hash == buffer[i + 2]) {
                    // Launch the callback
                    frame->hash[IdxHash].pointer(frame->hash[IdxHash].obj, 
                            buffer[i + 1], buffer[i + 3], 
                            (OR_BUS_FRAME_packet_t*) & buffer[i + 4]);
                }
            }
        }
    }
    // Build the message and send
    OR_BUS_FRAME_build(frame);
}

void OR_BUS_FRAME_init(OR_BUS_FRAME_t *frame, unsigned char *buff) {
    unsigned int i;
    // Initialization hash map controller
    for (i = 0; i < OR_BUS_FRAME_LNG_HASH_DECODER; ++i) {
        frame->hash[i].hash = 0;
        frame->hash[i].pointer = NULL;
        frame->hash[i].obj = NULL;
    }
    // Initialization buffer
    frame->buff = buff;
    // Reset counter messages
    frame->counter = 0;
    // Initialization over BUS
    OR_BUS_init(&frame->or_bus, &frame->buffTx[0], &frame->buffRx[0], 
            OR_BUS_FRAME_LNG_FRAME, (void *)frame, &OR_BUS_FRAME_decoder);
}

bool OR_BUS_FRAME_register(OR_BUS_FRAME_t *frame, 
        OR_BUS_FRAME_hashmap_t hashmap, OR_BUS_FRAME_parser cb, void *obj) {
    unsigned int i;
    // Find an available hash map
    for(i = 0; i < OR_BUS_FRAME_LNG_HASH_DECODER; ++i) {
        if(frame->hash[i].pointer == NULL) {
            frame->hash[i].hash = hashmap;
            frame->hash[i].pointer = cb;
            frame->hash[i].obj = obj;
            return true;
        }
    }
    return false;
}
/**
 * 
 * @param frame
 * @param type
 * @param hashmap
 * @param command
 * @param packet
 * @param length
 */
void OR_BUS_FRAME_add(OR_BUS_FRAME_t *frame, OR_BUS_FRAME_type_t type, 
        OR_BUS_FRAME_hashmap_t hashmap, OR_BUS_FRAME_command_t command, 
        OR_BUS_FRAME_packet_t* packet, size_t length) {
    // Copy all frame information in frames buffer
    frame->buff[frame->counter]     = OR_BUS_LNG_PACKET_HEAD + length;
    frame->buff[frame->counter + 1] = type;
    frame->buff[frame->counter + 2] = hashmap;
    frame->buff[frame->counter + 3] = command;
    // Copy the message
    if (type == OR_BUS_FRAME_DATA) {
        memcpy(&frame->buff[frame->counter + OR_BUS_LNG_PACKET_HEAD], packet, length);
    }
    // Update counter size message
    frame->counter += OR_BUS_LNG_PACKET_HEAD + length;
}

void OR_BUS_FRAME_add_data(OR_BUS_FRAME_t *frame, OR_BUS_FRAME_hashmap_t hashmap, 
        OR_BUS_FRAME_command_t command, OR_BUS_FRAME_packet_t* packet, size_t length) {
    OR_BUS_FRAME_add(frame, OR_BUS_FRAME_DATA, hashmap, command, packet, length);
}

void OR_BUS_FRAME_add_request(OR_BUS_FRAME_t *frame, OR_BUS_FRAME_type_t type, 
        OR_BUS_FRAME_hashmap_t hashmap, OR_BUS_FRAME_command_t command) {
    OR_BUS_FRAME_add(frame, type, hashmap, command, NULL, 0);
}

bool OR_BUS_FRAME_build(OR_BUS_FRAME_t *frame) {
    // Check are required to sent a message
    if(frame->counter > 0) {
        OR_BUS_build(&frame->or_bus, frame->buff, frame->counter);
        // reset the frame counter
        frame->counter = 0;
        return true;
    } else 
        return false;
}