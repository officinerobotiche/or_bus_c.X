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

#ifndef OR_FRAME_H
#define	OR_FRAME_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "packet/packet.h"
#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <string.h>
    
/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/
    // Dimension of list messages to decode in a packet
    #define BUFFER_LIST_PARSING 10
    /// function to decode packet
    typedef void (*frame_reader_t)(packet_information_t*, size_t*, packet_information_t*);
    
    #define CREATE_PACKET_DATA(cmd, type, data) createPacket((cmd), PACKET_DATA, (type), &(data), sizeof(data))
    #define CREATE_PACKET_RESPONSE(cmd, type, x) createPacket((cmd), (x), (type), NULL, 0)
    #define CREATE_PACKET_ACK(cmd,type) CREATE_PACKET_RESPONSE(cmd, type, PACKET_ACK)
    #define CREATE_PACKET_NACK(cmd,type) CREATE_PACKET_RESPONSE(cmd, type, PACKET_NACK)

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/
    /**
     * Init hashmap for decode messages
     * Load all hashmaps from packet/packet.h and packet/unav.h
     */
    void orb_frame_init();

    void set_frame_reader(unsigned char hash, frame_reader_t send, frame_reader_t receive);

    /**
     * In a packet we have more messages. A typical data packet
     * have this struct:
     * -------------------------- ---------------------------- -----------------------
     * | Length | CMD | DATA ... | Length | CMD | INFORMATION |Length | CMD | ... ... |
     * -------------------------- ---------------------------- -----------------------
     *    1        2 -> length    length+1 length+2 length+3   ...
     * It is possible to have different type of messages:
     * * Message with data (D)
     * * Message witn state information:
     *      * (R) request data
     *      * (A) ack
     *      * (N) nack
     * We have tre parts to elaborate and send a new packet (if required)
     * 1. [SAVING] The first part of this function split packets in a
     * list of messages to compute.
     * 2. [COMPUTE] If message have a data in tail, start compute and return a
     * new ACK or NACK message to append in a new packet. If is a request
     * message (R), the new message have in tail the data required.
     * 3. [SEND] Encoding de messages and transform in a packet to send.
     * *This function is a long function*
     * @return time to compute parsing packet
     */
    inline bool parser(packet_t* receive_pkg, packet_information_t* list_to_send, size_t* len);

    /**
     * Get a list of messages to transform in a packet for serial communication.
     * This function create a new packet and copy with UNION buffer_packet_u and
     * finally put chars conversion in to buffer.
     * @param list_send pointer of list with messages to send
     * @param len length of list_send list
     * @return a packet_t with all data to send
     */
    packet_t encoder(packet_information_t *list_send, size_t len);

    /**
     * Get an information_packet to convert in a buffer of char to put
     * in a packet_t data.
     * @param list_send information_packet_t to send
     * @return a packet_t with data to send
     */
    packet_t encoderSingle(packet_information_t list_send);

    /**
     * Create a information_packet_t associated a message listed in
     * abstract_message_t (see packet/packet.h for all listed messages).
     * Finally add information about message.
     * @param command type of message to send
     * @param option information about this message
     * @param type type of message
     * @param packet abstract_message to convert in a information_packet
     * @param size of packet
     * @return information_packet ready to send
     */
    inline packet_information_t createPacket(unsigned char command, unsigned char option, unsigned char type, message_abstract_u * packet, size_t len);
    /**
     * Create an information packet for a message with data (D).
     * This function use createPacket for create information_packet
     * @param command information about this message
     * @param type type of command to send
     * @param packet abstract_message to convert in a information_packet
     * @param size of packet
     * @return information_packet ready to send
     */
    inline packet_information_t createDataPacket(unsigned char command, unsigned char type, message_abstract_u * packet, size_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* OR_FRAME_H */
