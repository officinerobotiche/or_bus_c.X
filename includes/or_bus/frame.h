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

#include "or_bus/bus.h"
#include "packet/frame_diff_drive.h"
#include "packet/frame_motor.h"
#include "packet/frame_system.h"
#include "packet/frame_navigation.h"
#include "packet/frame_peripherals.h"
      
#ifndef OR_BUS_FRAME_LNG_HASH_DECODER
#define OR_BUS_FRAME_LNG_HASH_DECODER 5
#endif
    
/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/
    
// Length of information packet (without data)
#define OR_BUS_LNG_PACKET_HEAD 4
    /**
     * Definition of type of frame message required
     */
    typedef enum {
        OR_BUS_FRAME_NACK = 'N',
        OR_BUS_FRAME_ACK = 'K',
        OR_BUS_FRAME_DATA = 'D',
        OR_BUS_FRAME_REQUEST = 'R',
        OR_BUS_FRAME_EMPTY = 'E'
    } OR_BUS_FRAME_type_t;
    /**
     * The hash map message
     */
    typedef char OR_BUS_FRAME_hashmap_t;
    /**
     * The Command received
     */
    typedef char OR_BUS_FRAME_command_t;
    /**
     * Union for conversion all type of packets in a standard packets
     */
    typedef union _OR_BUS_FRAME_packet {
        system_frame_u system;
        motor_frame_u motor;
        diff_drive_frame_u diff_drive;
        navigation_frame_u sensor;
        peripherals_gpio_frame_u gpio;
    } OR_BUS_FRAME_packet_t;
#define OR_BUS_FRAME_LNG_PACKET sizeof(OR_BUS_FRAME_packet_t)
    
    typedef void (*OR_BUS_FRAME_parser)(void* obj, OR_BUS_FRAME_type_t, OR_BUS_FRAME_command_t, OR_BUS_FRAME_packet_t*);
    
    typedef struct _over_frame_hash {
        OR_BUS_FRAME_hashmap_t hash;
        OR_BUS_FRAME_parser pointer;
        void* obj;
    } over_frame_hash_t;
    
#define OR_BUS_FRAME_LNG_FRAME OR_BUS_LNG_HEADER + (5 * (OR_BUS_LNG_PACKET_HEAD + OR_BUS_FRAME_LNG_PACKET))
    
    typedef struct _over_frame {
        OR_BUS_t or_bus;
        unsigned int counter;
        over_frame_hash_t hash[OR_BUS_FRAME_LNG_HASH_DECODER];
    } OR_BUS_FRAME_t;

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/
    /**
     * @brief Initialization OR BUS controller with frame decoder
     * @param frame The frame controller
     * @param buffTx The transmitter buffer
     * @param buffRx The receiver buffer 
     * @param rx_size the size of received buffer
     */
    void OR_BUS_FRAME_init(OR_BUS_FRAME_t *frame, unsigned char *buffTx, 
        unsigned char *buffRx, unsigned int rx_size);
    /**
     * @brief Register a type of hash map.
     * @param frame The frame controller
     * @param hashmap the type of hash map
     * @param cb The callback lo launch after decoding
     * @param obj The object to launch inside the callback
     * @return If is available hash map space return true
     */
    bool OR_BUS_FRAME_register(OR_BUS_FRAME_t *frame, 
            OR_BUS_FRAME_hashmap_t hashmap, OR_BUS_FRAME_parser cb, void *obj);
    /**
     * @brief Add data in a frame message inside the buffer.
     * @param frame The frame controller
     * @param hashmap The associated hash map
     * @param command The command request
     * @param packet The packet to add
     * @param length The length of the packet
     * @return If available space in the buffer return true
     */
    bool OR_BUS_FRAME_add_data(OR_BUS_FRAME_t *frame, OR_BUS_FRAME_hashmap_t hashmap, 
        OR_BUS_FRAME_command_t command, OR_BUS_FRAME_packet_t* packet, size_t length);
    /**
     * @brief Add a request frame message inside the buffer.
     * @param frame The frame controller
     * @param type The type of message
     * @param hashmap The associated hash map
     * @param command The command request
     * @return If available space in the buffer return true
     */
    bool OR_BUS_FRAME_add_request(OR_BUS_FRAME_t *frame, OR_BUS_FRAME_type_t type, 
        OR_BUS_FRAME_hashmap_t hashmap, OR_BUS_FRAME_command_t command);
    /**
     * @brief Decode a OR_BUS message
     * @param The OR_BUS controller
     * @param rxchar character received from interrupt
     * @return the status of the decoder
     */
    OR_BUS_State_t OR_BUS_FRAME_decoder(OR_BUS_FRAME_t *frame, unsigned char rxchar);
    /**
     * @brief Build a OR BUS Frame packet.
     * @param frame The frame controller
     * @return return true if is available some frame to send
     */
    bool OR_BUS_FRAME_build(OR_BUS_FRAME_t *frame);

#ifdef	__cplusplus
}
#endif

#endif	/* OR_FRAME_H */
